#!/usr/bin/env bash
# build-linux.sh - Build reprodutível do LibertyRecomp no Arch Linux
# Uso: ./build-linux.sh [--no-deps] [--no-build]
#      --no-deps  pula a instalação de pacotes do sistema
#      --no-build só aplica os patches, sem compilar
set -euo pipefail

LIBERTY_COMMIT="91e55836"
O1HEAP_COMMIT="388a73f"
MODULES_DIR="${HOME}/.cmake-modules"
WORKSPACE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SKIP_DEPS=0
SKIP_BUILD=0

for arg in "$@"; do
  case "$arg" in
    --no-deps)  SKIP_DEPS=1 ;;
    --no-build) SKIP_BUILD=1 ;;
    --help|-h)
      echo "Uso: $0 [--no-deps] [--no-build]"
      exit 0 ;;
  esac
done

info()  { printf '\033[1;32m[INFO]\033[0m %s\n' "$*"; }
warn()  { printf '\033[1;33m[WARN]\033[0m %s\n' "$*"; }
error() { printf '\033[1;31m[ERR ]\033[0m %s\n' "$*" >&2; exit 1; }

cd "${WORKSPACE}"
[ -d .git ] || error "Não é um repositório git: ${WORKSPACE}"

# ─── 1. Dependências do sistema (Arch) ────────────────────────────────────
if [ "${SKIP_DEPS}" -eq 0 ]; then
  info "Verificando dependências..."
  MISSING=()
  for pkg in cmake ninja clang glslang libxss python git unzip curl; do
    pacman -Q "$pkg" &>/dev/null || MISSING+=("$pkg")
  done
  if [ ${#MISSING[@]} -gt 0 ]; then
    info "Instalando: ${MISSING[*]}"
    sudo pacman -S --needed --noconfirm "${MISSING[@]}"
  else
    info "Todas as dependências já instaladas."
  fi
fi

# ─── 2. FindPythonInterp.cmake (CMake 4.x removeu) ────────────────────────
mkdir -p "${MODULES_DIR}"
if [ ! -f "${MODULES_DIR}/FindPythonInterp.cmake" ]; then
  info "Baixando FindPythonInterp.cmake..."
  curl -fsSL -o "${MODULES_DIR}/FindPythonInterp.cmake" \
    "https://raw.githubusercontent.com/Kitware/CMake/v3.26.0/Modules/FindPythonInterp.cmake"
else
  info "FindPythonInterp.cmake já presente."
fi

# ─── 2.5. Submódulos (clone direto por URL+commit, ignora .gitmodules quebrado) ─
# O .gitmodules do upstream está incompleto: vários paths têm gitlink mas sem URL.
# Solução: lista hardcoded com (path|url|commit) extraída de um checkout funcional.
SUBMODULES=(
  "thirdparty/msdf-atlas-gen|https://github.com/Chlumsky/msdf-atlas-gen.git|7e8d34645a67c6e4def5d281f7adec1c2501dc49"
  "thirdparty/nativefiledialog-extended|https://github.com/btzy/nativefiledialog-extended.git|388549a5badaa7cbd138f5f189f50c67d5bf060c"
  "thirdparty/plume|https://github.com/renderbag/plume|519e3258048fe0bc65044e75fba7b1543e4c8e90"
  "thirdparty/ffmpeg-core|https://github.com/sonicnext-dev/ffmpeg-core|5089f001a5442308599fbab8e35282f5ad8ccf13"
  "thirdparty/GameNetworkingSockets|https://github.com/ValveSoftware/GameNetworkingSockets.git|517fff0cf6866ba163f4f016b0ef28f365c06c05"
  "thirdparty/ddspp|https://github.com/redorav/ddspp.git|98ce1d384706c8d7121876742a786f4eb89a23ef"
  "tools/XenosRecomp|https://github.com/sonicnext-dev/XenosRecomp.git|fb32631ee398e46f2a113d8f9103201dbaa000b4"
  "glue/rexglue-sdk-main/thirdparty/sdl3|https://github.com/libsdl-org/SDL|0fb1177405bb4c35a6f0b7991744f98293ac27fc"
  "glue/rexglue-sdk-main/thirdparty/glslang|https://github.com/KhronosGroup/glslang.git|f4f1d8a352ca1908943aea2ad8c54b39b4879080"
  "glue/rexglue-sdk-main/thirdparty/spirv-tools|https://github.com/KhronosGroup/SPIRV-Tools.git|04d0b166dcd62e29509bf2aac3ca0c5ccdcb6929"
  "glue/rexglue-sdk-main/thirdparty/spirv-headers|https://github.com/KhronosGroup/SPIRV-Headers.git|04f10f650d514df88b76d25e83db360142c7b174"
  "glue/rexglue-sdk-main/thirdparty/imgui|https://github.com/ocornut/imgui.git|6d910d5487d11ca567b61c7824b0c78c569d62f0"
  "glue/rexglue-sdk-main/thirdparty/fmt|https://github.com/fmtlib/fmt.git|407c905e45ad75fc29bf0f9bb7c5c2fd3475976f"
  "glue/rexglue-sdk-main/thirdparty/spdlog|https://github.com/gabime/spdlog.git|79524ddd08a4ec981b7fea76afd08ee05f83755d"
  "glue/rexglue-sdk-main/thirdparty/tracy|https://github.com/wolfpld/tracy.git|05cceee0df3b8d7c6fa87e9638af311dbabc63cb"
  "glue/rexglue-sdk-main/thirdparty/volk|https://github.com/zeux/volk.git|0b17a763ba5643e32da1b2152f8140461b3b7345"
  "glue/rexglue-sdk-main/thirdparty/vulkan-headers|https://github.com/KhronosGroup/Vulkan-Headers.git|49f1a381e2aec33ef32adf4a377b5a39ec016ec4"
  "glue/rexglue-sdk-main/thirdparty/vulkan-memory-allocator|https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git|1d8f600fd424278486eade7ed3e877c99f0846b1"
  "glue/rexglue-sdk-main/thirdparty/xxHash|https://github.com/Cyan4973/xxHash.git|e626a72bc2321cd320e953a0ccf1584cad60f363"
  "glue/rexglue-sdk-main/thirdparty/simde|https://github.com/simd-everywhere/simde.git|71fd833d9666141edcd1d3c109a80e228303d8d7"
  "glue/rexglue-sdk-main/thirdparty/tomlplusplus|https://github.com/marzer/tomlplusplus.git|30172438cee64926dc41fdd9c11fb3ba5b2ba9de"
  "glue/rexglue-sdk-main/thirdparty/utfcpp|https://github.com/nemtrif/utfcpp.git|63d64de49fd6b829f7c8694df5ab2ee625cb7134"
  "glue/rexglue-sdk-main/thirdparty/inja|https://github.com/pantor/inja.git|7d1b4600b68595085a949743331c2e5673f511ea"
  "glue/rexglue-sdk-main/thirdparty/cli11|https://github.com/CLIUtils/CLI11|bfffd37e1f804ca4fae1caae106935791696b6a9"
  "glue/rexglue-sdk-main/thirdparty/catch2|https://github.com/catchorg/Catch2.git|88abf9bf325c798c33f54f6b9220ef885b267f4f"
  "glue/rexglue-sdk-main/thirdparty/snappy|https://github.com/google/snappy.git|6af9287fbdb913f0794d0148c6aa43b58e63c8e3"
  "glue/rexglue-sdk-main/thirdparty/libmspack|https://github.com/kyz/libmspack.git|305907723a4e7ab2018e58040059ffb5e77db837"
  "glue/rexglue-sdk-main/thirdparty/FFmpeg|https://github.com/wmarti/FFmpeg.git|0604b464c7cb4ebc94940cf1f324a3b26b87717c"
    # ── LibertyRecomp (raiz) terceiros ────────────────────────────────────────
  "thirdparty/imgui|https://github.com/ocornut/imgui.git|8199457a7d9e453f8d3d9cadc14683fb54a858b5"
  "thirdparty/implot|https://github.com/epezent/implot.git|77674d270e851d3f3718aad00234201af2b76ac9"
  "thirdparty/json|https://github.com/nlohmann/json|606b6347edf0758c531abb6c36743e09a4c48a84"
  "thirdparty/magic_enum|https://github.com/Neargye/magic_enum.git|1a1824df7ac798177a521eed952720681b0bf482"
  "thirdparty/stb|https://github.com/nothings/stb.git|5c205738c191bcb0abc65c4febfa9bd25ff35234"
  "thirdparty/unordered_dense|https://github.com/martinus/unordered_dense.git|d911053e390816ecc5dedd5a9d6b4bb5ed92b4c9"
  "thirdparty/concurrentqueue|https://github.com/cameron314/concurrentqueue.git|6dd38b8a1dbaa7863aa907045f32308a56a6ff5d"
)

# vcpkg primeiro (é o toolchain do CMake)
if [ ! -f thirdparty/vcpkg/scripts/buildsystems/vcpkg.cmake ]; then
  info "Inicializando vcpkg..."
  git submodule update --init --recursive thirdparty/vcpkg 2>&1 | tail -2
fi

# Clona cada submódulo por URL+commit (não depende do .gitmodules)
CLONED=0
for entry in "${SUBMODULES[@]}"; do
  IFS='|' read -r path url commit <<< "$entry"
  if [ ! -d "$path" ] || [ -z "$(ls -A "$path" 2>/dev/null | grep -v '^\.git$')" ]; then
    info "  clone $path"
    rm -rf "$path"
    mkdir -p "$path"
    (
      cd "$path" || exit 1
      git init -q
      git remote add origin "$url"

      # Fetch: tenta depth 1 (rápido), fallback para full
      fetched=0
      if git fetch -q --depth 1 origin "$commit" 2>/dev/null; then
        git checkout -q FETCH_HEAD 2>/dev/null && fetched=1
      fi
      if [ "$fetched" -eq 0 ]; then
        git fetch -q origin 2>/dev/null
        git checkout -q "$commit" 2>/dev/null && fetched=1
      fi
      if [ "$fetched" -eq 0 ]; then
        warn "    FALHOU checkout $commit em $path"
        exit 1
      fi

      # Sub-submódulos: se o repo tem .gitmodules, inicializa recursivamente
      if [ -f .gitmodules ]; then
        git submodule update --init --recursive --quiet 2>/dev/null || \
          warn "    sub-submódulos falharam em $path (pode continuar)"
      fi
    ) || warn "  clone incompleto: $path"
    CLONED=$((CLONED+1))
  fi
done

# ─── 3. Checkout do commit alinhado com o SDK ─────────────────────────────
CURRENT_BRANCH=$(git branch --show-current 2>/dev/null || echo "")
if [ "${CURRENT_BRANCH}" = "linux-build-fixes" ]; then
  info "Já no branch linux-build-fixes (patches commitados)."
elif git merge-base --is-ancestor "${LIBERTY_COMMIT}" HEAD 2>/dev/null; then
  info "HEAD já contém ${LIBERTY_COMMIT} (e patches)."
else
  CUR=$(git rev-parse --short HEAD)
  info "Checkout ${LIBERTY_COMMIT} (atual: ${CUR})..."
  git stash push -u -m "build-linux-autostash-$(date +%s)" 2>/dev/null || true
  git checkout "${LIBERTY_COMMIT}"
fi

# ─── 4. Patches CMake (idempotentes) ──────────────────────────────────────
info "Patches CMake..."
grep -q 'CMP0148' CMakeLists.txt || \
  sed -i '/^cmake_minimum_required/a cmake_policy(SET CMP0148 OLD)' CMakeLists.txt
grep -q 'find_package(OpenSSL' CMakeLists.txt || \
  sed -i '/^project(/a find_package(OpenSSL 3.0 REQUIRED COMPONENTS Crypto)' CMakeLists.txt
grep -q 'OpenSSL::Crypto' LibertyRecomp/CMakeLists.txt || \
  sed -i 's/^    liberty_debugger$/    liberty_debugger\n    OpenSSL::Crypto/' LibertyRecomp/CMakeLists.txt

# ─── 5. Patch rpf_extractor.cpp (OpenSSL no Linux) ────────────────────────
if ! grep -q 'openssl/evp.h' LibertyRecomp/install/rpf_extractor.cpp; then
  info "Patch rpf_extractor.cpp..."
  python3 - "${WORKSPACE}" <<'PYEOF'
import sys, re, os
ws = sys.argv[1]
p = os.path.join(ws, "LibertyRecomp/install/rpf_extractor.cpp")
src = open(p).read()

# 4a) include
src = src.replace(
    '#else\n#include <CommonCrypto/CommonCrypto.h>\n#endif',
    '#else\n// Linux/Unix: OpenSSL para descriptografia AES-256-ECB\n#include <openssl/evp.h>\n#endif',
    1)

# 4b) bloco OpenSSL no lugar do CommonCrypto
m = re.search(
    r'(#elif defined\(__ANDROID__\).*?return false;\n)#else\n(.*?)#endif\n    \}',
    src, re.S)
if not m:
    print("ERRO: bloco esperado não encontrado", file=sys.stderr); sys.exit(1)

openssl_block = m.group(1) + '''#elif defined(__APPLE__)
        // macOS/iOS CommonCrypto - decrypt 16 times like SparkIV
        for (int round = 0; round < 16; round++) {
            size_t outLength = 0;
            CCCryptorStatus status = CCCrypt(
                kCCDecrypt, kCCAlgorithmAES, kCCOptionECBMode,
                key.data(), kCCKeySizeAES256, nullptr,
                data.data(), dataLen, data.data(), dataLen, &outLength);
            if (status != kCCSuccess) return false;
        }
        return true;
#else
        // Linux/Unix: OpenSSL AES-256-ECB, sem padding
        if (dataLen % 16 != 0) return false;
        for (int round = 0; round < 16; round++) {
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (!ctx) return false;
            int outLen = 0, finalLen = 0;
            bool ok = true;
            ok &= (EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), nullptr, key.data(), nullptr) == 1);
            ok &= (EVP_CIPHER_CTX_set_padding(ctx, 0) == 1);
            ok &= (EVP_DecryptUpdate(ctx, data.data(), &outLen, data.data(), (int)dataLen) == 1);
            ok &= (EVP_DecryptFinal_ex(ctx, data.data() + outLen, &finalLen) == 1);
            EVP_CIPHER_CTX_free(ctx);
            if (!ok) return false;
        }
        return true;
#endif
    }'''

src = src[:m.start()] + openssl_block + src[m.end():]
open(p, "w").write(src)
print("OK: rpf_extractor.cpp patchado")
PYEOF
else
  info "rpf_extractor.cpp já patchado."
fi

# ─── 6. o1heap v3.0 (verificação defensiva) ───────────────────────────────
cd glue/rexglue-sdk-main/thirdparty/o1heap
# o upstream tem o código dentro de uma subpasta "o1heap/"
if [ ! -f o1heap/o1heap.h ]; then
  warn "o1heap sem código — clonando do upstream..."
  cd ..
  rm -rf o1heap
  git clone https://github.com/pavel-kirienko/o1heap.git o1heap
  cd o1heap
  git checkout 388a73f 2>/dev/null || git checkout 3.0.0
  rm -rf .git
else
  info "o1heap já presente (v3.0 esperado)."
fi
cd "${WORKSPACE}"

# ─── 7. sources.cmake stub (se codegen ainda não rodou) ───────────────────
if [ ! -f glue/rexglue-sdk-main/gta4-recomp/generated/sources.cmake ]; then
  warn "sources.cmake ausente — criando stub (build sem código do jogo)."
  mkdir -p glue/rexglue-sdk-main/gta4-recomp/generated
  echo 'set(GENERATED_SOURCES "")' > glue/rexglue-sdk-main/gta4-recomp/generated/sources.cmake
fi

# ─── 8. Variáveis vcpkg/CMake 4 ───────────────────────────────────────────
export VCPKG_ABI_TOOLS_USE_EXACT_VERSIONS=1
export CMAKE_POLICY_VERSION_MINIMUM=3.5

# ─── 9. Configure + build ─────────────────────────────────────────────────
if [ "${SKIP_BUILD}" -eq 0 ]; then
  info "Configurando..."
  rm -rf out/build/linux-release
  cmake --preset linux-release \
    -DSDL_X11_XSCRNSAVER=OFF \
    -DCMAKE_MODULE_PATH="${MODULES_DIR}" \
    -DGTA4_NATIVE_GLSLANG_VALIDATOR=/usr/bin/glslangValidator

  info "Compilando..."
  cmake --build --preset linux-release --parallel "$(nproc)"
  info "Build concluído."
  info "Binário: ${WORKSPACE}/out/build/linux-release/LibertyRecomp"
fi

info "Pronto."
