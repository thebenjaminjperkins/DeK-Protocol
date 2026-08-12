cmake_minimum_required(VERSION 3.16)
project(dek_protocol C)

set(DEK_PROTOCOL_ROOT "${CMAKE_CURRENT_LIST_DIR}")
include("${DEK_PROTOCOL_ROOT}/cmake/dek_protocol_sources.cmake")

add_library(dek_protocol STATIC)

target_sources(dek_protocol PRIVATE ${DEK_PROTOCOL_SOURCES})

target_include_directories(dek_protocol
    PUBLIC
        ${DEK_PROTOCOL_INCLUDE_DIR}
)

target_compile_features(dek_protocol PUBLIC c_std_11)

enable_testing()
add_subdirectory(tests)
