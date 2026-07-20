# DeK Protocol Boundaries

This repository defines the DeK wire protocol.

It is platform independent.

## Allowed

- Packet encoding
- Packet decoding
- CRC
- Message definitions
- Shared enums
- Shared constants
- Serialization
- Deserialization

## Forbidden

- ESP-IDF
- Pico SDK
- FreeRTOS
- UART
- USB
- SPI
- GPIO
- DMA
- Hardware access
- Operating system APIs