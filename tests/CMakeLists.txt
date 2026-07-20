add_executable(packet_test packet_test.c)

target_link_libraries(packet_test
    PRIVATE
        dek_protocol
)