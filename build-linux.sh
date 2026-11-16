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

# ─── 2.5. Submódulos de terceiros (SDL3, glslang, spirv-tools, etc.) ──────
if [ ! -f glue/rexglue-sdk-main/thirdparty/sdl3/CMakeLists.txt ] || \
   [ ! -f glue/rexglue-sdk-main/thirdparty/glslang/CMakeLists.txt ]; then
  info "Inicializando submódulos (pode demorar na primeira vez)..."
  git submodule update --init --recursive 2>&1 | \
    grep -E "Submodule|Cloning|fatal" || true
else
  info "Submódulos já inicializados."
fi

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
