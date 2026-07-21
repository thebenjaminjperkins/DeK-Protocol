# DeK-Protocol

DeK-Protocol is the shared protocol library used by every DeK device.

It defines the language spoken between the DeK host and every attached module.
Its purpose is to ensure that every implementation, regardless of processor,
operating system, or transport, uses the same packet format, message
definitions, serialization logic, validation, and protocol behavior.

The library is intentionally platform-independent.

It does not know about ESP-IDF, Pico SDK, FreeRTOS, SPI peripherals, UART
drivers, DMA, interrupts, displays, or application logic.

Instead, it provides a common protocol implementation that can be compiled into:

- ESP32 host firmware
- RP2350 modules
- Desktop simulators
- Unit tests
- Future Linux/macOS tooling
- Protocol analyzers
- Automated integration tests

The goal is that there is only one implementation of the DeK protocol.

# Goals

## Single Source of Truth

The protocol should never have multiple implementations that drift apart.

Every packet definition, message structure, encoder, decoder, checksum, and
validation rule should live here.

If a protocol change occurs, both the host and module receive it automatically
by updating this library.

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

## Deterministic Behavior

Given identical input bytes, every implementation must produce identical
results.

This makes debugging, automated testing, desktop simulation, and protocol
verification much easier.

## Versioned Protocol

The protocol should evolve without breaking existing hardware.

Future protocol versions should be able to coexist.

Example:

```text
Protocol V1
Host V1 <-> Module V1

Protocol V2
Host V2 <-> Module V2

Host V2 <-> Module V1
```

Compatibility rules belong inside this library.

# Responsibilities

DeK-Protocol owns everything related to protocol semantics.

That includes:

- Packet format
- Header layout
- CRC
- Message identifiers
- Shared control-plane payload structures
- Shared capability contract definitions
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

# Non-Responsibilities

DeK-Protocol intentionally does not own:

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

# Current Scope

At the current stage of development, DeK-Protocol exposes the core packet and
transport implementation plus shared control-plane and capability contracts.

## Packet Layer

Responsible for:

- packet header definitions
- packet layout
- encoding
- decoding
- validation

## Message Definitions

Defines every protocol message ID and shared message payload structure.

## Transport

Owns:

- sequence numbers
- packet construction
- statistics
- validation
- timeout bookkeeping

The transport should remain generic.

It should never contain module-specific logic.

## CRC

Shared checksum implementation.

Every packet uses exactly the same CRC implementation.

## Shared Capability Contracts

Capability IDs, versions, command IDs, and payload layouts that both the host
and modules must agree on belong here.

Current examples include:

- `uart.stream`
- `gpio.digital`

## Packet Receiver

A streaming parser capable of consuming bytes one at a time.

Responsibilities:

- synchronize
- detect magic bytes
- assemble packets
- validate CRC
- return decoded packets

It should know nothing about message meaning.

# Design Philosophy

DeK-Protocol should remain small, deterministic, and completely
hardware-agnostic.

It should never contain board-specific code, transport drivers, or application
logic.

Instead, it defines the language spoken by every DeK device.

Every host, module, simulator, and future desktop utility should compile
against the same protocol implementation, ensuring consistent behavior across
the entire ecosystem.
