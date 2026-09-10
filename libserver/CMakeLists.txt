cmake_minimum_required(VERSION 3.20)
project(libserver LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenSSL 3.0 REQUIRED COMPONENTS SSL Crypto)
find_package(Threads REQUIRED)

function(libserver_enable_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endfunction()

add_library(libserver_json INTERFACE)
target_include_directories(libserver_json INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

add_library(libserver_http_runtime STATIC src/http_runtime.cpp src/deferred_responses.cpp)
target_include_directories(libserver_http_runtime PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(libserver_http_runtime PUBLIC OpenSSL::SSL Threads::Threads)
if(WIN32)
  target_link_libraries(libserver_http_runtime PUBLIC ws2_32)
endif()
libserver_enable_warnings(libserver_http_runtime)

add_library(libserver_persistent_state STATIC src/persistent_state.cpp)
target_include_directories(libserver_persistent_state PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(libserver_persistent_state PUBLIC libserver_json)
libserver_enable_warnings(libserver_persistent_state)

add_library(libserver_gta4_stat_schema STATIC src/gta4_stat_schema.cpp)
target_include_directories(libserver_gta4_stat_schema PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src")
libserver_enable_warnings(libserver_gta4_stat_schema)

add_executable(libserver src/main.cpp)
target_link_libraries(libserver PRIVATE libserver_http_runtime libserver_persistent_state
    libserver_gta4_stat_schema libserver_json OpenSSL::SSL OpenSSL::Crypto Threads::Threads)
libserver_enable_warnings(libserver)

if(BUILD_TESTING)
  find_package(Python3 REQUIRED COMPONENTS Interpreter)

  add_executable(libserver_persistent_state_test tests/persistent_state_test.cpp)
  target_link_libraries(libserver_persistent_state_test PRIVATE libserver_persistent_state)
  libserver_enable_warnings(libserver_persistent_state_test)

  add_executable(libserver_http_runtime_test tests/http_runtime_test.cpp)
  target_link_libraries(libserver_http_runtime_test PRIVATE libserver_http_runtime)
  libserver_enable_warnings(libserver_http_runtime_test)

  add_executable(libserver_gta4_stat_schema_test tests/gta4_stat_schema_test.cpp)
  target_link_libraries(libserver_gta4_stat_schema_test PRIVATE libserver_gta4_stat_schema)
  libserver_enable_warnings(libserver_gta4_stat_schema_test)

  add_executable(libserver_identity_contract_test tests/identity_contract_test.cpp)
  target_include_directories(libserver_identity_contract_test PRIVATE src)
  target_link_libraries(libserver_identity_contract_test PRIVATE libserver_json)
  libserver_enable_warnings(libserver_identity_contract_test)
  add_test(NAME libserver_identity_contract_test COMMAND libserver_identity_contract_test)

  add_executable(libserver_service_recovery_test tests/service_recovery_test.cpp)
  target_link_libraries(libserver_service_recovery_test PRIVATE libserver_http_runtime
      libserver_persistent_state libserver_gta4_stat_schema libserver_json
      OpenSSL::SSL OpenSSL::Crypto Threads::Threads)
  libserver_enable_warnings(libserver_service_recovery_test)
  add_test(NAME libserver_service_recovery_test COMMAND libserver_service_recovery_test)

  add_test(NAME libserver_reliability_regression
      COMMAND "${Python3_EXECUTABLE}"
              "${CMAKE_CURRENT_SOURCE_DIR}/tests/server_reliability_regression_test.py"
              "$<TARGET_FILE:libserver>")
  set_tests_properties(libserver_reliability_regression PROPERTIES TIMEOUT 180)

  add_executable(libserver_state_regression_test tests/server_state_regression_test.cpp)
  target_link_libraries(libserver_state_regression_test PRIVATE libserver_http_runtime
      libserver_persistent_state libserver_gta4_stat_schema libserver_json
      OpenSSL::SSL OpenSSL::Crypto Threads::Threads)
  libserver_enable_warnings(libserver_state_regression_test)
  add_test(NAME libserver_state_regression_test COMMAND libserver_state_regression_test)

  add_test(NAME libserver_self_test COMMAND libserver --self-test)
  add_test(NAME libserver_persistent_state_test COMMAND libserver_persistent_state_test)
  add_test(NAME libserver_http_runtime_test COMMAND libserver_http_runtime_test)
  add_test(NAME libserver_gta4_stat_schema_test COMMAND libserver_gta4_stat_schema_test)
  add_test(NAME libserver_runtime_limit_math
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/runtime_limit_math.py")
  add_test(NAME libserver_resource_lease_math
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/derive_resource_leases.py")
  add_test(NAME libserver_multiplayer_64_math
      COMMAND "${Python3_EXECUTABLE}"
              "${CMAKE_CURRENT_SOURCE_DIR}/tools/derive_multiplayer_64_validation.py")
  add_test(NAME libserver_multiplayer_64_simulation COMMAND libserver --simulate-mp64)
  add_test(NAME libserver_qos_limit_math
      COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/derive_qos_limits.py")
  add_test(NAME libserver_gta4_stat_schema_generation
      COMMAND "${Python3_EXECUTABLE}"
              "${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_gta4_stat_schema.py" --check)
  add_test(NAME libserver_gta4_profile_contract
      COMMAND "${Python3_EXECUTABLE}"
              "${CMAKE_CURRENT_SOURCE_DIR}/tools/derive_gta4_profile_contract.py")
  add_test(NAME libserver_gta4_rank_thresholds
      COMMAND "${Python3_EXECUTABLE}"
              "${CMAKE_CURRENT_SOURCE_DIR}/tools/derive_rank_thresholds.py")
  add_test(NAME libserver_social_contract
      COMMAND "${Python3_EXECUTABLE}"
              "${CMAKE_CURRENT_SOURCE_DIR}/tools/derive_social_contract.py")
  add_test(NAME libserver_prog_ach_contract
      COMMAND "${Python3_EXECUTABLE}"
              "${CMAKE_CURRENT_SOURCE_DIR}/tools/derive_prog_ach_contract.py")
  add_test(NAME libserver_folder_self_contained
      COMMAND "${Python3_EXECUTABLE}"
              "${CMAKE_CURRENT_SOURCE_DIR}/tests/folder_self_contained_test.py"
              "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_COMMAND}"
              "${CMAKE_SYSTEM_PROCESSOR}")
  set_tests_properties(libserver_folder_self_contained PROPERTIES TIMEOUT 5220)
  if(UNIX)
    add_test(NAME libserver_http_server_integration_test
        COMMAND "${Python3_EXECUTABLE}"
                "${CMAKE_CURRENT_SOURCE_DIR}/tests/http_server_integration_test.py"
                "$<TARGET_FILE:libserver>")
    set_tests_properties(libserver_http_server_integration_test PROPERTIES SKIP_RETURN_CODE 77)
    add_test(NAME libserver_active_client_e2e_test
        COMMAND "${Python3_EXECUTABLE}"
                "${CMAKE_CURRENT_SOURCE_DIR}/tests/active_client_e2e_test.py"
                "$<TARGET_FILE:libserver>")
    set_tests_properties(libserver_active_client_e2e_test PROPERTIES
        SKIP_RETURN_CODE 77 TIMEOUT 1200)
    add_test(NAME libserver_social_lobby_e2e_test
        COMMAND "${Python3_EXECUTABLE}"
                "${CMAKE_CURRENT_SOURCE_DIR}/tests/social_lobby_e2e_test.py"
                "$<TARGET_FILE:libserver>")
    set_tests_properties(libserver_social_lobby_e2e_test PROPERTIES
        SKIP_RETURN_CODE 77 TIMEOUT 480)
    add_test(NAME libserver_deferred_tls_regression
        COMMAND "${Python3_EXECUTABLE}"
                "${CMAKE_CURRENT_SOURCE_DIR}/tests/deferred_tls_regression_test.py"
                "$<TARGET_FILE:libserver>")
    set_tests_properties(libserver_deferred_tls_regression PROPERTIES TIMEOUT 60)
    add_test(NAME libserver_production_hosting_test
        COMMAND "${Python3_EXECUTABLE}"
                "${CMAKE_CURRENT_SOURCE_DIR}/tests/production_hosting_test.py"
                "$<TARGET_FILE:libserver>")
    set_tests_properties(libserver_production_hosting_test PROPERTIES
        SKIP_RETURN_CODE 77 TIMEOUT 120)
  endif()
endif()

install(TARGETS libserver RUNTIME DESTINATION bin)
install(FILES third_party/LICENSE DESTINATION share/licenses/libserver
        RENAME nlohmann-json-LICENSE)
