cmake_minimum_required(VERSION 3.16)

project(dek-protocol C)

add_library(dek-protocol STATIC
    src/dek_crc.c
    src/dek_packet.c
    src/dek_transport.c

    src/messages/dek_hello.c
)

target_include_directories(dek-protocol
    PUBLIC
        include
)