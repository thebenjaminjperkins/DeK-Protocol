cmake_minimum_required(VERSION 3.16)

project(dek_protocol C)

add_library(dek_protocol STATIC
    src/dek_crc.c
    src/dek_packet.c
    src/dek_transport.c
    src/message-types/dek_hello.c
)

target_include_directories(dek_protocol
    PUBLIC
        include
)