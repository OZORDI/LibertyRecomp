#!/usr/bin/env node
// Math MCP Server for LibertyRecomp
// Specialized calculations for PPC recompilation work

import { Server } from '@modelcontextprotocol/sdk/server/index.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import {
  CallToolRequestSchema,
  ListToolsRequestSchema,
} from '@modelcontextprotocol/sdk/types.js';
import { MathTools } from './tools/math-tools.js';

const mathTools = new MathTools();

// Tool definitions
const TOOLS = [
  // Hex/Address Math
  {
    name: 'hex_to_dec',
    description: 'Convert hexadecimal to decimal. Shows both signed and unsigned interpretations.',
    inputSchema: {
      type: 'object',
      properties: {
        hex: { type: 'string', description: 'Hex value (e.g., "0x82A00000" or "82A00000")' },
        signed: { type: 'boolean', description: 'Return signed interpretation (default: true)' },
        bits: { type: 'number', description: 'Bit width (default: 32)' },
      },
      required: ['hex'],
    },
  },
  {
    name: 'dec_to_hex',
    description: 'Convert decimal to hexadecimal. Handles negative numbers via two\'s complement.',
    inputSchema: {
      type: 'object',
      properties: {
        decimal: { type: 'string', description: 'Decimal value (e.g., "-2101739520")' },
        bits: { type: 'number', description: 'Bit width (default: 32)' },
      },
      required: ['decimal'],
    },
  },
  {
    name: 'add_offset',
    description: 'Add an offset to a base address. Essential for PPC memory calculations.',
    inputSchema: {
      type: 'object',
      properties: {
        base: { type: 'string', description: 'Base address (hex or decimal)' },
        offset: { type: 'string', description: 'Offset to add (can be negative)' },
      },
      required: ['base', 'offset'],
    },
  },
  {
    name: 'address_range',
    description: 'Calculate address range from start address and size.',
    inputSchema: {
      type: 'object',
      properties: {
        start: { type: 'string', description: 'Start address' },
        size: { type: 'string', description: 'Size in bytes' },
      },
      required: ['start', 'size'],
    },
  },
  
  // PPC-Specific Math
  {
    name: 'lis_calculation',
    description: 'Calculate PPC lis (load immediate shifted) instruction result. lis loads upper 16 bits.',
    inputSchema: {
      type: 'object',
      properties: {
        immediate: { type: 'string', description: 'The 16-bit immediate value (e.g., "-32070" or "0x82A0")' },
      },
      required: ['immediate'],
    },
  },
  {
    name: 'effective_address',
    description: 'Calculate PPC effective address from lis value + offset. Common pattern: lis rX,upper; lwz rY,offset(rX)',
    inputSchema: {
      type: 'object',
      properties: {
        lis_value: { type: 'string', description: 'The lis result or immediate (e.g., "-2101739520" or "-32070")' },
        offset: { type: 'string', description: 'The offset from lwz/addi instruction (e.g., "-15952")' },
      },
      required: ['lis_value', 'offset'],
    },
  },
  {
    name: 'ppc_offset_decode',
    description: 'Decode a PPC offset value showing 16-bit and 32-bit signed interpretations.',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: 'The offset value to decode' },
      },
      required: ['value'],
    },
  },
  
  // Signed/Unsigned Conversions
  {
    name: 'twos_complement',
    description: 'Convert between signed and unsigned using two\'s complement.',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: 'Value to convert (hex or decimal)' },
        bits: { type: 'number', description: 'Bit width (default: 32)' },
      },
      required: ['value'],
    },
  },
  {
    name: 'sign_extend',
    description: 'Sign-extend a value from one bit width to another.',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: 'Value to extend' },
        from_bits: { type: 'number', description: 'Source bit width' },
        to_bits: { type: 'number', description: 'Target bit width' },
      },
      required: ['value', 'from_bits', 'to_bits'],
    },
  },
  
  // Bit Operations
  {
    name: 'bit_mask',
    description: 'Create a bit mask from start to end bit (inclusive, 0-indexed from LSB).',
    inputSchema: {
      type: 'object',
      properties: {
        start_bit: { type: 'number', description: 'Start bit (0-indexed from LSB)' },
        end_bit: { type: 'number', description: 'End bit (inclusive)' },
        bits: { type: 'number', description: 'Total bit width (default: 32)' },
      },
      required: ['start_bit', 'end_bit'],
    },
  },
  {
    name: 'bit_shift',
    description: 'Perform bit shift operations (left, right, logical, arithmetic).',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: 'Value to shift' },
        amount: { type: 'number', description: 'Shift amount' },
        direction: { type: 'string', enum: ['left', 'right'], description: 'Shift direction' },
        logical: { type: 'boolean', description: 'Logical shift (true) or arithmetic (false). Default: true' },
        bits: { type: 'number', description: 'Bit width (default: 32)' },
      },
      required: ['value', 'amount', 'direction'],
    },
  },
  {
    name: 'bit_extract',
    description: 'Extract bits from a value starting at a given position.',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: 'Value to extract from' },
        start_bit: { type: 'number', description: 'Starting bit position (0-indexed from LSB)' },
        num_bits: { type: 'number', description: 'Number of bits to extract' },
      },
      required: ['value', 'start_bit', 'num_bits'],
    },
  },
  {
    name: 'rlwinm',
    description: 'PPC rlwinm instruction: Rotate Left Word Immediate then AND with Mask.',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: 'Value to rotate' },
        shift: { type: 'number', description: 'Rotate amount (SH)' },
        mask_begin: { type: 'number', description: 'Mask begin bit (MB, 0=MSB)' },
        mask_end: { type: 'number', description: 'Mask end bit (ME, 0=MSB)' },
      },
      required: ['value', 'shift', 'mask_begin', 'mask_end'],
    },
  },
  
  // Timing Math
  {
    name: 'hz_to_ms',
    description: 'Convert frequency (Hz) to period (milliseconds). Useful for VBlank/frame timing.',
    inputSchema: {
      type: 'object',
      properties: {
        hz: { type: 'number', description: 'Frequency in Hz (e.g., 60 for 60Hz)' },
      },
      required: ['hz'],
    },
  },
  {
    name: 'fps_calculator',
    description: 'Convert between FPS and frame time.',
    inputSchema: {
      type: 'object',
      properties: {
        input: { type: 'number', description: 'Input value' },
        input_type: { type: 'string', enum: ['fps', 'ms', 'us'], description: 'Type of input value' },
      },
      required: ['input', 'input_type'],
    },
  },
  {
    name: 'timing_analysis',
    description: 'Analyze frame timing against target FPS budget.',
    inputSchema: {
      type: 'object',
      properties: {
        target_fps: { type: 'number', description: 'Target FPS (e.g., 60)' },
        actual_frame_time_ms: { type: 'number', description: 'Actual frame time in milliseconds' },
      },
      required: ['target_fps', 'actual_frame_time_ms'],
    },
  },
  
  // Struct/Memory Layout
  {
    name: 'struct_size',
    description: 'Infer struct size from known field offsets.',
    inputSchema: {
      type: 'object',
      properties: {
        offsets: { type: 'array', items: { type: 'number' }, description: 'Array of known field offsets' },
      },
      required: ['offsets'],
    },
  },
  {
    name: 'align_address',
    description: 'Align an address to a given boundary.',
    inputSchema: {
      type: 'object',
      properties: {
        address: { type: 'string', description: 'Address to align' },
        alignment: { type: 'number', description: 'Alignment boundary (e.g., 4, 8, 16)' },
      },
      required: ['address', 'alignment'],
    },
  },
  {
    name: 'page_align',
    description: 'Align a size to page boundaries.',
    inputSchema: {
      type: 'object',
      properties: {
        size: { type: 'string', description: 'Size to align' },
        page_size: { type: 'number', description: 'Page size (default: 4096)' },
      },
      required: ['size'],
    },
  },
  
  // General Arithmetic
  {
    name: 'calculate',
    description: 'Evaluate a math expression. Supports: + - * / % ** << >> & | ^ ~. Hex (0x) and binary (0b) literals supported.',
    inputSchema: {
      type: 'object',
      properties: {
        expression: { type: 'string', description: 'Math expression (e.g., "0x82A00000 + -15952" or "1 << 16")' },
      },
      required: ['expression'],
    },
  },
  {
    name: 'base_convert',
    description: 'Convert a number between bases (2, 8, 10, 16).',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: 'Value to convert' },
        from_base: { type: 'number', description: 'Source base (2, 8, 10, 16)' },
        to_base: { type: 'number', description: 'Target base (2, 8, 10, 16)' },
      },
      required: ['value', 'from_base', 'to_base'],
    },
  },

  // ==================== NEW TOOLS ====================

  // Memory Region Analysis
  {
    name: 'ppc_memory_map',
    description: 'Describe a memory address - what region, type, and usage.',
    inputSchema: {
      type: 'object',
      properties: {
        address: { type: 'string', description: 'Memory address to describe' },
      },
      required: ['address'],
    },
  },
  {
    name: 'round_to_page',
    description: 'Round a size up to page boundary. Uses formula: (value + pageSize - 1) & ~(pageSize - 1)',
    inputSchema: {
      type: 'object',
      properties: {
        size: { type: 'string', description: 'Size to round up' },
        page_size: { type: 'number', description: 'Page size (default: 4096)' },
      },
      required: ['size'],
    },
  },
  {
    name: 'is_valid_ppc_address',
    description: 'Check if an address is in valid PPC range (0x82000000-0x84000000) and identify its region.',
    inputSchema: {
      type: 'object',
      properties: {
        address: { type: 'string', description: 'Address to validate' },
      },
      required: ['address'],
    },
  },

  // Xbox 360 Performance Counter
  {
    name: 'perf_ticks_to_ms',
    description: 'Convert Xbox 360 performance counter ticks to milliseconds (÷ 49875000 Hz).',
    inputSchema: {
      type: 'object',
      properties: {
        ticks: { type: 'string', description: 'Performance counter ticks' },
      },
      required: ['ticks'],
    },
  },
  {
    name: 'ms_to_perf_ticks',
    description: 'Convert milliseconds to Xbox 360 performance counter ticks (× 49875000 Hz / 1000).',
    inputSchema: {
      type: 'object',
      properties: {
        ms: { type: 'number', description: 'Milliseconds' },
      },
      required: ['ms'],
    },
  },
  {
    name: 'timebase_to_seconds',
    description: 'Convert Xbox 360 timebase (mftb instruction) to seconds.',
    inputSchema: {
      type: 'object',
      properties: {
        timebase: { type: 'string', description: 'Timebase value from mftb' },
      },
      required: ['timebase'],
    },
  },

  // Byte Swapping / Endianness
  {
    name: 'byte_swap_16',
    description: 'Swap bytes of a 16-bit value (big ↔ little endian).',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: '16-bit value to swap' },
      },
      required: ['value'],
    },
  },
  {
    name: 'byte_swap_32',
    description: 'Swap bytes of a 32-bit value (big ↔ little endian).',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: '32-bit value to swap' },
      },
      required: ['value'],
    },
  },
  {
    name: 'byte_swap_64',
    description: 'Swap bytes of a 64-bit value (big ↔ little endian).',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: '64-bit value to swap' },
      },
      required: ['value'],
    },
  },
  {
    name: 'byte_swap_float',
    description: 'Swap bytes of an IEEE 754 float (for GPU/shader data endianness conversion).',
    inputSchema: {
      type: 'object',
      properties: {
        value: { type: 'string', description: 'Float as hex (e.g., "0x3F800000" for 1.0)' },
      },
      required: ['value'],
    },
  },

  // NTSTATUS / Error Code Analysis
  {
    name: 'ntstatus_decode',
    description: 'Decode an NTSTATUS code to name, severity, and description.',
    inputSchema: {
      type: 'object',
      properties: {
        status: { type: 'string', description: 'NTSTATUS code (e.g., "0xC0000008")' },
      },
      required: ['status'],
    },
  },
  {
    name: 'ntstatus_is_error',
    description: 'Check if an NTSTATUS is an error (severity 3, 0xC0000000 prefix).',
    inputSchema: {
      type: 'object',
      properties: {
        status: { type: 'string', description: 'NTSTATUS code to check' },
      },
      required: ['status'],
    },
  },
  {
    name: 'ntstatus_is_warning',
    description: 'Check if an NTSTATUS is a warning (severity 2, 0x80000000 prefix but not 0xC0000000).',
    inputSchema: {
      type: 'object',
      properties: {
        status: { type: 'string', description: 'NTSTATUS code to check' },
      },
      required: ['status'],
    },
  },

  // Allocation / Pool Math
  {
    name: 'allocation_units',
    description: 'Convert bytes to allocation units (for XFILE_FS_SIZE_INFORMATION).',
    inputSchema: {
      type: 'object',
      properties: {
        bytes: { type: 'string', description: 'Size in bytes' },
        sectors_per_unit: { type: 'number', description: 'Sectors per allocation unit (default: 8)' },
        bytes_per_sector: { type: 'number', description: 'Bytes per sector (default: 512)' },
      },
      required: ['bytes'],
    },
  },
  {
    name: 'sectors_to_bytes',
    description: 'Convert sectors to bytes.',
    inputSchema: {
      type: 'object',
      properties: {
        sectors: { type: 'string', description: 'Number of sectors' },
        bytes_per_sector: { type: 'number', description: 'Bytes per sector (default: 512)' },
      },
      required: ['sectors'],
    },
  },
];

async function main() {
  const server = new Server(
    { name: 'math-tools', version: '1.1.0' },
    { capabilities: { tools: {} } }
  );

  console.error('Math MCP Server starting...');

  server.setRequestHandler(ListToolsRequestSchema, async () => ({ tools: TOOLS }));

  server.setRequestHandler(CallToolRequestSchema, async (request) => {
    const { name, arguments: args } = request.params;

    try {
      let result: unknown;

      switch (name) {
        // Hex/Address Math
        case 'hex_to_dec':
          result = mathTools.hexToDec(
            args?.hex as string,
            args?.signed as boolean ?? true,
            (args?.bits as number) ?? 32
          );
          break;
        case 'dec_to_hex':
          result = mathTools.decToHex(args?.decimal as string, (args?.bits as number) ?? 32);
          break;
        case 'add_offset':
          result = mathTools.addOffset(args?.base as string, args?.offset as string);
          break;
        case 'address_range':
          result = mathTools.addressRange(args?.start as string, args?.size as string);
          break;

        // PPC-Specific Math
        case 'lis_calculation':
          result = mathTools.lisCalculation(args?.immediate as string);
          break;
        case 'effective_address':
          result = mathTools.effectiveAddress(args?.lis_value as string, args?.offset as string);
          break;
        case 'ppc_offset_decode':
          result = mathTools.ppcOffsetDecode(args?.value as string);
          break;

        // Signed/Unsigned Conversions
        case 'twos_complement':
          result = mathTools.twosComplement(args?.value as string, (args?.bits as number) ?? 32);
          break;
        case 'sign_extend':
          result = mathTools.signExtend(
            args?.value as string,
            args?.from_bits as number,
            args?.to_bits as number
          );
          break;

        // Bit Operations
        case 'bit_mask':
          result = mathTools.bitMask(
            args?.start_bit as number,
            args?.end_bit as number,
            (args?.bits as number) ?? 32
          );
          break;
        case 'bit_shift':
          result = mathTools.bitShift(
            args?.value as string,
            args?.amount as number,
            args?.direction as 'left' | 'right',
            args?.logical as boolean ?? true,
            (args?.bits as number) ?? 32
          );
          break;
        case 'bit_extract':
          result = mathTools.bitExtract(
            args?.value as string,
            args?.start_bit as number,
            args?.num_bits as number
          );
          break;
        case 'rlwinm':
          result = mathTools.rlwinm(
            args?.value as string,
            args?.shift as number,
            args?.mask_begin as number,
            args?.mask_end as number
          );
          break;

        // Timing Math
        case 'hz_to_ms':
          result = mathTools.hzToMs(args?.hz as number);
          break;
        case 'fps_calculator':
          result = mathTools.fpsCalculator(args?.input as number, args?.input_type as 'fps' | 'ms' | 'us');
          break;
        case 'timing_analysis':
          result = mathTools.timingAnalysis(args?.target_fps as number, args?.actual_frame_time_ms as number);
          break;

        // Struct/Memory Layout
        case 'struct_size':
          result = mathTools.structSize(args?.offsets as number[]);
          break;
        case 'align_address':
          result = mathTools.alignAddress(args?.address as string, args?.alignment as number);
          break;
        case 'page_align':
          result = mathTools.pageAlign(args?.size as string, (args?.page_size as number) ?? 4096);
          break;

        // General Arithmetic
        case 'calculate':
          result = mathTools.calculate(args?.expression as string);
          break;
        case 'base_convert':
          result = mathTools.baseConvert(
            args?.value as string,
            args?.from_base as number,
            args?.to_base as number
          );
          break;

        // ==================== NEW TOOL HANDLERS ====================

        // Memory Region Analysis
        case 'ppc_memory_map':
          result = mathTools.ppcMemoryMap(args?.address as string);
          break;
        case 'round_to_page':
          result = mathTools.roundToPage(args?.size as string, (args?.page_size as number) ?? 4096);
          break;
        case 'is_valid_ppc_address':
          result = mathTools.isValidPpcAddress(args?.address as string);
          break;

        // Xbox 360 Performance Counter
        case 'perf_ticks_to_ms':
          result = mathTools.perfTicksToMs(args?.ticks as string);
          break;
        case 'ms_to_perf_ticks':
          result = mathTools.msToPerfTicks(args?.ms as number);
          break;
        case 'timebase_to_seconds':
          result = mathTools.timebaseToSeconds(args?.timebase as string);
          break;

        // Byte Swapping / Endianness
        case 'byte_swap_16':
          result = mathTools.byteSwap16(args?.value as string);
          break;
        case 'byte_swap_32':
          result = mathTools.byteSwap32(args?.value as string);
          break;
        case 'byte_swap_64':
          result = mathTools.byteSwap64(args?.value as string);
          break;
        case 'byte_swap_float':
          result = mathTools.byteSwapFloat(args?.value as string);
          break;

        // NTSTATUS / Error Code Analysis
        case 'ntstatus_decode':
          result = mathTools.ntstatusDecode(args?.status as string);
          break;
        case 'ntstatus_is_error':
          result = mathTools.ntstatusIsError(args?.status as string);
          break;
        case 'ntstatus_is_warning':
          result = mathTools.ntstatusIsWarning(args?.status as string);
          break;

        // Allocation / Pool Math
        case 'allocation_units':
          result = mathTools.allocationUnits(
            args?.bytes as string,
            (args?.sectors_per_unit as number) ?? 8,
            (args?.bytes_per_sector as number) ?? 512
          );
          break;
        case 'sectors_to_bytes':
          result = mathTools.sectorsToBytes(args?.sectors as string, (args?.bytes_per_sector as number) ?? 512);
          break;

        default:
          return { content: [{ type: 'text', text: `Unknown tool: ${name}` }], isError: true };
      }

      return { content: [{ type: 'text', text: JSON.stringify(result, null, 2) }] };
    } catch (error) {
      return { content: [{ type: 'text', text: `Error: ${error}` }], isError: true };
    }
  });

  const transport = new StdioServerTransport();
  await server.connect(transport);
  console.error('Math MCP Server running on stdio');
}

main().catch(console.error);
