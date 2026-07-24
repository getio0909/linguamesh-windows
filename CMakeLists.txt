cmake_minimum_required(VERSION 3.24)

project(LinguaMeshWindowsReference LANGUAGES CXX)

add_library(linguamesh_windows_reference
    src/core/core_bridge.cpp
    src/app/app_model.cpp
    src/platform/credential_store.cpp
    src/platform/windows_credential_store.cpp
)

target_compile_features(linguamesh_windows_reference PUBLIC cxx_std_20)
target_include_directories(linguamesh_windows_reference PUBLIC include)

if(MSVC)
    target_compile_options(linguamesh_windows_reference PRIVATE /W4 /permissive- /EHsc)
else()
    target_compile_options(linguamesh_windows_reference PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(linguamesh_windows_reference_tests
    tests/reference_slice_tests.cpp
)
target_link_libraries(linguamesh_windows_reference_tests PRIVATE linguamesh_windows_reference)
target_compile_features(linguamesh_windows_reference_tests PRIVATE cxx_std_20)

enable_testing()
add_test(NAME linguamesh_windows_reference_tests COMMAND linguamesh_windows_reference_tests)
