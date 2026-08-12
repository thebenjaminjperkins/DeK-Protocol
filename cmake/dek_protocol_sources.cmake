if(NOT DEFINED DEK_PROTOCOL_ROOT)
    message(FATAL_ERROR "DEK_PROTOCOL_ROOT must be set before including dek_protocol_sources.cmake")
endif()

set(DEK_PROTOCOL_INCLUDE_DIR "${DEK_PROTOCOL_ROOT}/include")

set(DEK_PROTOCOL_SOURCES
    "${DEK_PROTOCOL_ROOT}/src/dek_crc.c"
    "${DEK_PROTOCOL_ROOT}/src/dek_packet.c"
    "${DEK_PROTOCOL_ROOT}/src/dek_receiver.c"
    "${DEK_PROTOCOL_ROOT}/src/dek_transport.c"
    "${DEK_PROTOCOL_ROOT}/src/message-types/dek_hello.c"
    "${DEK_PROTOCOL_ROOT}/src/message-types/dek_descriptor.c"
    "${DEK_PROTOCOL_ROOT}/src/message-types/dek_capabilities.c"
    "${DEK_PROTOCOL_ROOT}/src/message-types/dek_open_capability.c"
    "${DEK_PROTOCOL_ROOT}/src/message-types/dek_service_command.c"
    "${DEK_PROTOCOL_ROOT}/src/capabilities/dek_uart_stream.c"
    "${DEK_PROTOCOL_ROOT}/src/capabilities/dek_gpio_digital.c"
)
