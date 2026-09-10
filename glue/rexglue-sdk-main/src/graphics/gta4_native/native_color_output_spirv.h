#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include <spirv/unified1/spirv.hpp11>

#include "native_color_output.h"

namespace rex::graphics::gta4_native {
namespace color_output_spirv {
using Words = std::vector<uint32_t>;
constexpr uint32_t U(spv::Op op) { return uint32_t(op); }

inline void Emit(Words& to, spv::Op op, std::span<const uint32_t> args) {
  to.push_back((uint32_t(args.size() + 1) << 16) | U(op));
  to.insert(to.end(), args.begin(), args.end());
}
inline void Emit(Words& to, spv::Op op, std::initializer_list<uint32_t> args) {
  Emit(to, op, std::span<const uint32_t>(args.begin(), args.size()));
}
inline void EmitString(Words& to, spv::Op op, const char* value) {
  Words bytes;
  uint32_t word = 0, shift = 0;
  for (;;) {
    const uint32_t c = uint8_t(*value++);
    word |= c << shift;
    if (!c || shift == 24) { bytes.push_back(word); word = 0; shift = 0; }
    else { shift += 8; }
    if (!c) break;
  }
  Emit(to, op, bytes);
}

struct Module {
  std::vector<Words> instructions;
  std::map<Words, uint32_t> declarations;
  Words additions;
  uint32_t next_id = 0;
  uint32_t Id() { return next_id++; }
  uint32_t Type(spv::Op op, std::initializer_list<uint32_t> args) {
    Words key{U(op)};
    key.insert(key.end(), args.begin(), args.end());
    if (auto i = declarations.find(key); i != declarations.end()) return i->second;
    const uint32_t id = Id();
    Words operands{id};
    operands.insert(operands.end(), args.begin(), args.end());
    Emit(additions, op, operands);
    declarations.emplace(std::move(key), id);
    return id;
  }
  uint32_t Constant(uint32_t type, std::initializer_list<uint32_t> words) {
    Words key{U(spv::Op::OpConstant), type};
    key.insert(key.end(), words.begin(), words.end());
    if (auto i = declarations.find(key); i != declarations.end()) return i->second;
    const uint32_t id = Id();
    Words operands{type, id};
    operands.insert(operands.end(), words.begin(), words.end());
    Emit(additions, spv::Op::OpConstant, operands);
    declarations.emplace(std::move(key), id);
    return id;
  }
};
}  // namespace color_output_spirv

// Keep the original shader intact as a function, and make a new entry point that
// calls it once before transforming color outputs. Discard/termination still
// terminates the invocation. Original alpha testing, sample-mask generation and
// depth writes therefore run before output scaling. No material hash whitelist.
// Accept only the direct float32 color interface used by the admitted cache;
// malformed or unsupported modules fail rather than silently changing semantics.
inline std::optional<std::vector<uint32_t>> AddNativeColorOutputEpilogue(
    std::span<const uint32_t> input, std::string* error = nullptr) {
  using namespace color_output_spirv;
  auto fail = [&](const char* reason) -> std::optional<Words> {
    if (error) *error = reason;
    return std::nullopt;
  };
  if (input.size() < 5 || input[0] != spv::MagicNumber || !input[3] ||
      input[3] > 1000000 || input[4] != 0) return fail("invalid-module-header");
  Module m;
  m.next_id = input[3];
  const uint32_t bound = input[3];
  std::vector<Words> definitions(bound);
  std::vector<uint32_t> locations(bound, UINT32_MAX);
  std::vector<bool> builtin(bound), interface_ids(bound);
  uint32_t original_entry = 0, entry_count = 0, push_variable = 0, push_type = 0;
  bool int64_capability = false, physical_capability = false, physical_extension = false;
  size_t first_type = SIZE_MAX, first_function = SIZE_MAX, memory_model = SIZE_MAX;
  for (size_t p = 5; p < input.size();) {
    const uint32_t size = input[p] >> 16, opcode = input[p] & 0xFFFF;
    if (!size || size > input.size() - p) return fail("invalid-instruction-size");
    Words w(input.begin() + p, input.begin() + p + size);
    const auto op = spv::Op(opcode);
    if (op == spv::Op::OpEntryPoint) {
      if (size < 4 || w[1] != uint32_t(spv::ExecutionModel::Fragment) ||
          !w[2] || w[2] >= bound) return fail("not-a-fragment-entry-point");
      original_entry = w[2];
      ++entry_count;
      size_t start = 3;
      for (; start < w.size(); ++start) {
        const uint32_t x = w[start];
        if (!(x & 255u) || !(x & 65280u) || !(x & 16711680u) || !(x & 4278190080u)) {
          ++start;
          break;
        }
      }
      if (start > w.size()) return fail("unterminated-entry-name");
      for (; start < w.size(); ++start) {
        if (!w[start] || w[start] >= bound) return fail("invalid-interface-id");
        interface_ids[w[start]] = true;
      }
    } else if (op == spv::Op::OpCapability && size == 2) {
      int64_capability |= w[1] == uint32_t(spv::Capability::Int64);
      physical_capability |= w[1] == uint32_t(spv::Capability::PhysicalStorageBufferAddresses);
    } else if (op == spv::Op::OpExtension) {
      Words expected;
      EmitString(expected, op, "SPV_KHR_physical_storage_buffer");
      physical_extension |= w == expected;
    } else if (op == spv::Op::OpMemoryModel) {
      if (size != 3 || memory_model != SIZE_MAX) return fail("invalid-memory-model");
      memory_model = m.instructions.size();
    } else if (op == spv::Op::OpDecorate) {
      if (size < 3 || w[1] >= bound) return fail("invalid-decoration");
      if (w[2] == uint32_t(spv::Decoration::Location)) {
        if (size != 4 || locations[w[1]] != UINT32_MAX) return fail("invalid-location");
        locations[w[1]] = w[3];
      } else if (w[2] == uint32_t(spv::Decoration::BuiltIn)) {
        builtin[w[1]] = true;
      } else if ((w[2] == uint32_t(spv::Decoration::Component) ||
                  w[2] == uint32_t(spv::Decoration::Index)) && (size != 4 || w[3] != 0)) {
        return fail("unsupported-split-or-dual-source-interface");
      }
    }
    if (opcode >= U(spv::Op::OpTypeVoid) && opcode <= U(spv::Op::OpTypeFunction)) {
      if (size < 2 || !w[1] || w[1] >= bound) return fail("invalid-type");
      definitions[w[1]] = w;
      if (first_type == SIZE_MAX) first_type = m.instructions.size();
      Words key{opcode};
      key.insert(key.end(), w.begin() + 2, w.end());
      m.declarations.emplace(std::move(key), w[1]);
    } else if (op == spv::Op::OpConstant) {
      if (size < 4 || w[1] >= bound || !w[2] || w[2] >= bound) return fail("invalid-constant");
      Words key{opcode, w[1]};
      key.insert(key.end(), w.begin() + 3, w.end());
      m.declarations.emplace(std::move(key), w[2]);
    } else if (op == spv::Op::OpVariable) {
      if (size < 4 || w[1] >= bound || !w[2] || w[2] >= bound) return fail("invalid-variable");
      definitions[w[2]] = w;
      if (w[3] == uint32_t(spv::StorageClass::PushConstant)) {
        if (push_variable) return fail("multiple-push-constant-blocks");
        push_variable = w[2]; push_type = w[1];
      }
    } else if (op == spv::Op::OpFunction && first_function == SIZE_MAX) {
      first_function = m.instructions.size();
    }
    m.instructions.push_back(std::move(w));
    p += size;
  }
  if (entry_count != 1 || first_function == SIZE_MAX || first_type == SIZE_MAX ||
      memory_model == SIZE_MAX) return fail("incomplete-module");

  struct Output { uint32_t variable, type, components, location; };
  std::vector<Output> outputs;
  uint32_t output_mask = 0;
  for (uint32_t id = 1; id < bound; ++id) {
    const auto& v = definitions[id];
    if (!interface_ids[id] || builtin[id] || v.size() < 4 ||
        (v[0] & 0xFFFF) != U(spv::Op::OpVariable) ||
        v[3] != uint32_t(spv::StorageClass::Output)) continue;
    if (locations[id] >= kNativeColorOutputTargetCount ||
        (output_mask & (1u << locations[id]))) return fail("unsupported-output-location");
    const auto& ptr = definitions[v[1]];
    if (ptr.size() != 4 || (ptr[0] & 0xFFFF) != U(spv::Op::OpTypePointer) ||
        ptr[3] >= bound) return fail("invalid-output-pointer");
    const auto& ty = definitions[ptr[3]];
    if (ty.size() < 3) return fail("invalid-output-type");
    uint32_t components = 1, scalar = ptr[3];
    if ((ty[0] & 0xFFFF) == U(spv::Op::OpTypeVector)) {
      if (ty.size() != 4 || ty[2] >= bound || ty[3] < 2 || ty[3] > 4)
        return fail("unsupported-output-vector");
      scalar = ty[2]; components = ty[3];
    }
    const auto& f = definitions[scalar];
    if (f.size() != 3 || (f[0] & 0xFFFF) != U(spv::Op::OpTypeFloat) || f[2] != 32)
      return fail("color-output-is-not-float32");
    outputs.push_back({id, ptr[3], components, locations[id]});
    output_mask |= 1u << locations[id];
  }
  if (outputs.empty()) return Words(input.begin(), input.end());

  const uint32_t void_type = m.Type(spv::Op::OpTypeVoid, {});
  const uint32_t bool_type = m.Type(spv::Op::OpTypeBool, {});
  const uint32_t uint_type = m.Type(spv::Op::OpTypeInt, {32, 0});
  const uint32_t ulong_type = m.Type(spv::Op::OpTypeInt, {64, 0});
  const uint32_t float_type = m.Type(spv::Op::OpTypeFloat, {32});
  const uint32_t vec4_type = m.Type(spv::Op::OpTypeVector, {float_type, 4});
  const uint32_t float_zero = m.Constant(float_type, {0});
  const uint32_t index_two = m.Constant(uint_type, {2});
  const uint32_t push_ulong_ptr = m.Type(spv::Op::OpTypePointer,
      {uint32_t(spv::StorageClass::PushConstant), ulong_type});
  const uint32_t physical_vec4_ptr = m.Type(spv::Op::OpTypePointer,
      {uint32_t(spv::StorageClass::PhysicalStorageBuffer), vec4_type});
  Words annotations;
  const bool new_push = push_variable == 0;
  if (new_push) {
    const uint32_t structure = m.Type(spv::Op::OpTypeStruct, {ulong_type, ulong_type, ulong_type});
    push_type = m.Type(spv::Op::OpTypePointer,
        {uint32_t(spv::StorageClass::PushConstant), structure});
    push_variable = m.Id();
    Emit(m.additions, spv::Op::OpVariable,
         {push_type, push_variable, uint32_t(spv::StorageClass::PushConstant)});
    Emit(annotations, spv::Op::OpDecorate, {structure, uint32_t(spv::Decoration::Block)});
    for (uint32_t i = 0; i < 3; ++i)
      Emit(annotations, spv::Op::OpMemberDecorate,
           {structure, i, uint32_t(spv::Decoration::Offset), i * 8});
  } else {
    const auto& ptr = definitions[push_type];
    if (ptr.size() != 4 || ptr[3] >= bound) return fail("invalid-push-pointer");
    const uint32_t structure_id = ptr[3];
    const auto& structure = definitions[structure_id];
    if (structure.size() < 5 || (structure[0] & 0xFFFF) != U(spv::Op::OpTypeStruct) ||
        structure[4] != ulong_type) return fail("incompatible-shared-address-member");
    bool offset_ok = false;
    for (const auto& w : m.instructions)
      if (w.size() == 5 && (w[0] & 0xFFFF) == U(spv::Op::OpMemberDecorate) &&
          w[1] == structure_id && w[2] == 2 && w[3] == uint32_t(spv::Decoration::Offset))
        offset_ok = w[4] == 16;
    if (!offset_ok) return fail("incompatible-shared-address-offset");
  }
  const uint32_t wrapper = m.Id();
  const uint32_t function_type = m.Type(spv::Op::OpTypeFunction, {void_type});
  Words function;
  Emit(function, spv::Op::OpFunction,
       {void_type, wrapper, uint32_t(spv::FunctionControlMask::MaskNone), function_type});
  Emit(function, spv::Op::OpLabel, {m.Id()});
  Emit(function, spv::Op::OpFunctionCall, {void_type, m.Id(), original_entry});
  const uint32_t address_pointer = m.Id(), address = m.Id();
  Emit(function, spv::Op::OpAccessChain,
       {push_ulong_ptr, address_pointer, push_variable, index_two});
  Emit(function, spv::Op::OpLoad, {ulong_type, address, address_pointer});
  for (const auto& output : outputs) {
    const uint32_t n = output.components, ty = output.type;
    const uint32_t bty = n == 1 ? bool_type : m.Type(spv::Op::OpTypeVector, {bool_type, n});
    uint32_t zero = float_zero;
    if (n != 1) {
      zero = m.Id();
      Words operands{ty, zero}; operands.insert(operands.end(), n, float_zero);
      Emit(m.additions, spv::Op::OpConstantComposite, operands);
    }
    auto read_parameter = [&](uint32_t lane) {
      const uint32_t offset = kNativeColorOutputOffset +
          output.location * sizeof(NativeColorOutputParameters) + lane * 16;
      const uint32_t c = m.Constant(ulong_type, {offset, 0});
      const uint32_t p = m.Id(), pointer = m.Id(), vector = m.Id();
      Emit(function, spv::Op::OpIAdd, {ulong_type, p, address, c});
      Emit(function, spv::Op::OpConvertUToPtr, {physical_vec4_ptr, pointer, p});
      Emit(function, spv::Op::OpLoad,
           {vec4_type, vector, pointer, uint32_t(spv::MemoryAccessMask::Aligned), 16});
      if (n == 4) return vector;
      const uint32_t result = m.Id();
      if (n == 1) Emit(function, spv::Op::OpCompositeExtract, {ty, result, vector, 0});
      else {
        Words operands{ty, result, vector, vector};
        for (uint32_t i = 0; i < n; ++i) operands.push_back(i);
        Emit(function, spv::Op::OpVectorShuffle, operands);
      }
      return result;
    };
    const uint32_t scale = read_parameter(0), lower = read_parameter(1), upper = read_parameter(2);
    const uint32_t raw = m.Id(), scaled = m.Id(), bounded = m.Id(), nan = m.Id();
    const uint32_t finite = m.Id(), below = m.Id(), low_clamped = m.Id();
    const uint32_t above = m.Id(), clamped = m.Id(), result = m.Id();
    Emit(function, spv::Op::OpLoad, {ty, raw, output.variable});
    Emit(function, spv::Op::OpFMul, {ty, scaled, raw, scale});
    Emit(function, spv::Op::OpFOrdLessThanEqual, {bty, bounded, lower, upper});
    Emit(function, spv::Op::OpIsNan, {bty, nan, scaled});
    Emit(function, spv::Op::OpSelect, {ty, finite, nan, zero, scaled});
    Emit(function, spv::Op::OpFOrdLessThan, {bty, below, finite, lower});
    Emit(function, spv::Op::OpSelect, {ty, low_clamped, below, lower, finite});
    Emit(function, spv::Op::OpFOrdGreaterThan, {bty, above, low_clamped, upper});
    Emit(function, spv::Op::OpSelect, {ty, clamped, above, upper, low_clamped});
    Emit(function, spv::Op::OpSelect, {ty, result, bounded, clamped, scaled});
    Emit(function, spv::Op::OpStore, {output.variable, result});
  }
  Emit(function, spv::Op::OpReturn, {});
  Emit(function, spv::Op::OpFunctionEnd, {});
  Words result(input.begin(), input.begin() + 5);
  result[3] = m.next_id;
  if (!int64_capability) Emit(result, spv::Op::OpCapability, {uint32_t(spv::Capability::Int64)});
  if (!physical_capability) Emit(result, spv::Op::OpCapability,
                                {uint32_t(spv::Capability::PhysicalStorageBufferAddresses)});
  bool extension_inserted = physical_extension;
  for (size_t i = 0; i < m.instructions.size(); ++i) {
    Words w = m.instructions[i];
    const auto op = spv::Op(w[0] & 0xFFFF);
    if (!extension_inserted && op != spv::Op::OpCapability && op != spv::Op::OpExtension) {
      EmitString(result, spv::Op::OpExtension, "SPV_KHR_physical_storage_buffer");
      extension_inserted = true;
    }
    if (op == spv::Op::OpMemoryModel) w[1] = uint32_t(spv::AddressingModel::PhysicalStorageBuffer64);
    if (op == spv::Op::OpEntryPoint) {
      w[2] = wrapper;
      // SPIR-V 1.4 requires all used global variables in the interface list.
      if (input[1] >= 0x00010400 && new_push) {
        w.push_back(push_variable); w[0] = (uint32_t(w.size()) << 16) | U(op);
      }
    } else if ((op == spv::Op::OpExecutionMode || op == spv::Op::OpExecutionModeId) &&
               w.size() >= 3 && w[1] == original_entry) w[1] = wrapper;
    if (i == first_type) result.insert(result.end(), annotations.begin(), annotations.end());
    if (i == first_function) result.insert(result.end(), m.additions.begin(), m.additions.end());
    result.insert(result.end(), w.begin(), w.end());
  }
  result.insert(result.end(), function.begin(), function.end());
  return result;
}
}  // namespace rex::graphics::gta4_native
