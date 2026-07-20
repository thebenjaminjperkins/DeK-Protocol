cmake_minimum_required(VERSION 3.16)
project(dek_protocol C)

add_library(dek_protocol STATIC)

target_sources(dek_protocol PRIVATE
    src/dek_crc.c
    src/dek_packet.c
    src/dek_receiver.c
    src/dek_transport.c
    src/message-types/dek_hello.c
)

target_include_directories(dek_protocol
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(dek_protocol PUBLIC c_std_11)

enable_testing()
add_subdirectory(tests)
