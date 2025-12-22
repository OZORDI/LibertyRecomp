project("o1heap")

add_library(o1heap "o1heap.h" "o1heap.c")
target_include_directories(o1heap PUBLIC ".")