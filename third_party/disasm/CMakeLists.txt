project("disasm")

add_library(disasm "ppc-dis.c" "disasm.c" "ppc.h")
target_include_directories(disasm PUBLIC .)
