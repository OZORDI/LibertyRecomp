# Compatibility shim for CMake 4.x, which removed the FindPythonInterp module.
#
# Vendored glslang (glue/rexglue-sdk-main/thirdparty/glslang) still calls
# find_package(PythonInterp 3 REQUIRED) unconditionally. This module maps the
# legacy variables onto the modern FindPython3 module so a plain
# `cmake --preset linux-release` keeps working on distributions that ship
# CMake 4 (e.g. Arch Linux).
#
# Provides: PYTHONINTERP_FOUND, PYTHON_EXECUTABLE,
#           PYTHON_VERSION_STRING, PYTHON_VERSION_MAJOR/MINOR/PATCH.

if(PYTHONINTERP_FOUND)
  return()
endif()

find_package(Python3 COMPONENTS Interpreter QUIET)

if(Python3_Interpreter_FOUND)
  set(PYTHON_EXECUTABLE "${Python3_EXECUTABLE}")
  set(PYTHON_VERSION_STRING "${Python3_VERSION}")
  set(PYTHON_VERSION_MAJOR "${Python3_VERSION_MAJOR}")
  set(PYTHON_VERSION_MINOR "${Python3_VERSION_MINOR}")
  set(PYTHON_VERSION_PATCH "${Python3_VERSION_PATCH}")
else()
  set(PYTHON_EXECUTABLE "")
  set(PYTHON_VERSION_STRING "")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PythonInterp
  REQUIRED_VARS PYTHON_EXECUTABLE
  VERSION_VAR PYTHON_VERSION_STRING)

mark_as_advanced(PYTHON_EXECUTABLE)
