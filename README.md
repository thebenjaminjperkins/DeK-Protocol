# DeK-Protocol

DeK-Protocol is the shared protocol library used by every DeK device.

It defines the language spoken between the DeK host and every attached module. Its purpose is to ensure that every implementation—regardless of processor, operating system, or transport—uses the exact same packet format, message definitions, serialization logic, validation, and protocol behavior.

The library is intentionally platform-independent.

It does **not** know about ESP-IDF, Pico SDK, FreeRTOS, SPI peripherals, UART drivers, DMA, interrupts, displays, or application logic.

Instead, it provides a common protocol implementation that can be compiled into:

- ESP32 host firmware
- RP2350 modules
- Desktop simulators
- Unit tests
- Future Linux/macOS tooling
- Protocol analyzers
- Automated integration tests

The goal is that there is only **one implementation of the DeK protocol**.

---

# Goals

DeK-Protocol exists to solve several problems.

## Single Source of Truth

The protocol should never have multiple implementations that slowly drift apart.

Every packet definition, message structure, encoder, decoder, checksum, and validation rule should live here.

If a protocol change occurs, both the host and module receive it automatically by updating this library.

---

## Platform Independence

The protocol library should compile anywhere a C compiler exists.

It should have no dependencies on:

- ESP-IDF
- Pico SDK
- FreeRTOS
- CMSIS
- HAL libraries
- Drivers
- Operating systems

The only required dependencies should be the C standard library.

---

## Deterministic Behavior

Given identical input bytes, every implementation must produce identical results.

This makes:

- debugging
- automated testing
- desktop simulation
- protocol verification

far easier.

---

## Versioned Protocol

The protocol should evolve without breaking existing hardware.

Future protocol versions should be able to coexist.

Example:

```
Protocol V1
Host V1 ↔ Module V1

Protocol V2
Host V2 ↔ Module V2

Host V2 ↔ Module V1
```

Compatibility rules belong inside this library.

---

# Responsibilities

DeK-Protocol owns everything related to protocol semantics.

That includes:

- Packet format
- Header layout
- CRC
- Message identifiers
- Payload structures
- Serialization
- Deserialization
- Packet validation
- Sequence numbers
- Protocol versions
- Error codes
- Transport state
- Message encoding
- Message decoding

If both the host and module must agree on something, it probably belongs here.

---

# Non-Responsibilities

DeK-Protocol intentionally does **not** own:

- SPI drivers
- UART drivers
- USB drivers
- DMA
- GPIO
- Interrupts
- Tasks
- Threads
- Mutexes
- Displays
- Module management
- Applications
- UI
- Capability routing

Those belong to the firmware projects.

---

# Current Scope

At the current stage of development, DeK-Protocol should expose only the core protocol implementation.

## Packet Layer

Responsible for:

- packet header definitions
- packet layout
- encoding
- decoding
- validation

Example:

```c
dek_packet_encode(...)
dek_packet_decode(...)
```

---

## Message Definitions

Defines every protocol message ID.

Example:

```c
DEK_MSG_HELLO
DEK_MSG_PING
DEK_MSG_PONG
```

Future messages are added here.

---

## Transport

Owns:

- sequence numbers
- packet construction
- statistics
- validation
- timeout bookkeeping

Example:

```c
dek_transport_send(...)
```

The transport should remain generic.

It should never contain protocol-specific logic.

---

## CRC

Shared checksum implementation.

Every packet uses exactly the same CRC implementation.

Example:

```c
dek_crc16(...)
```

---

## HELLO

Shared payload definition.

Example:

```c
dek_hello_payload_t

dek_hello_encode(...)
dek_hello_decode(...)
```

Both the host and module use identical serialization.

---

## Packet Receiver

A streaming parser capable of consuming bytes one at a time.

Responsibilities:

- synchronize
- detect magic bytes
- assemble packets
- validate CRC
- return decoded packets

It should know nothing about message meaning.

---

# Suggested Repository Layout

```
DeK-Protocol/

include/
    dek/

        protocol_version.h

        packet.h
        message.h
        transport.h

        crc.h

        hello.h

src/

    packet.c
    transport.c

    crc.c

    hello.c

tests/

docs/

README.md
CHANGELOG.md
LICENSE
```

---

# What It Currently Exposes

Today the public API should remain intentionally small.

```
Packet

    Encode
    Decode
    Validate

Transport

    Initialize
    Build packet
    Assign sequence numbers

Messages

    Message IDs

CRC

    Compute
    Verify

HELLO

    Payload
    Encode
    Decode

Receiver

    Feed byte
    Receive packet
```

Everything else belongs in the host or module firmware.

---

# Future Growth

As DeK evolves, this library should become the canonical protocol implementation.

Future additions include:

---

## PING

```
PING
PONG
```

---

## Module Enumeration

```
MODULE_INFO

MODULE_DESCRIPTOR

MODULE_CAPABILITIES
```

---

## Capability Discovery

```
CAPABILITY_LIST

CAPABILITY_QUERY

CAPABILITY_RESPONSE
```

---

## Resource Management

```
RESERVE

RELEASE

LEASE

LEASE_DENIED
```

---

## Streaming

```
STREAM_START

STREAM_STOP

STREAM_DATA

STREAM_STATUS
```

---

## GPIO

```
GPIO_READ

GPIO_WRITE

GPIO_CONFIG

GPIO_EVENT
```

---

## UART

```
UART_OPEN

UART_CLOSE

UART_WRITE

UART_DATA

UART_STATUS
```

---

## SPI

```
SPI_TRANSFER

SPI_CONFIG
```

---

## I²C

```
I2C_READ

I2C_WRITE

I2C_SCAN
```

---

## Logic Analyzer

```
CAPTURE_START

CAPTURE_STOP

CAPTURE_DATA
```

---

## Oscilloscope

```
SCOPE_START

SCOPE_STOP

SCOPE_DATA
```

---

## Firmware Updates

```
FW_BEGIN

FW_DATA

FW_VERIFY

FW_COMMIT

FW_ABORT
```

---

## Diagnostics

```
ERROR_REPORT

HEALTH

STATISTICS

LOG_MESSAGE
```

---

## Protocol Compatibility

Future versions will include compatibility helpers.

Examples:

```
Protocol negotiation

Feature negotiation

Version compatibility

Extension discovery
```

---

# Design Philosophy

DeK-Protocol should remain small, deterministic, and completely hardware-agnostic.

It should never contain board-specific code, transport drivers, or application logic.

Instead, it defines the language spoken by every DeK device.

Every host, module, simulator, and future desktop utility should compile against the same protocol implementation, ensuring consistent behavior across the entire ecosystem.

If the firmware is the operating system of DeK, then DeK-Protocol is its networking stack—the single shared definition of how every component communicates.