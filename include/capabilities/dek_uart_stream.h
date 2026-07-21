#ifndef DEK_UART_STREAM_H
#define DEK_UART_STREAM_H

#include <stdbool.h>
#include <stdint.h>

#include "capabilities/dek_capability_ids.h"

#define DEK_UART_STREAM_OPEN_CONFIG_SIZE 12u

typedef enum
{
    DEK_UART_STREAM_PARITY_NONE = 0,
    DEK_UART_STREAM_PARITY_EVEN = 1,
    DEK_UART_STREAM_PARITY_ODD = 2
} dek_uart_stream_parity_t;

typedef enum
{
    DEK_UART_STREAM_STOP_BITS_1 = 1,
    DEK_UART_STREAM_STOP_BITS_2 = 2
} dek_uart_stream_stop_bits_t;

typedef enum
{
    DEK_UART_STREAM_FLOW_CONTROL_NONE = 0,
    DEK_UART_STREAM_FLOW_CONTROL_RTS_CTS = 1
} dek_uart_stream_flow_control_t;

typedef enum
{
    DEK_UART_STREAM_CMD_START = 0x01,
    DEK_UART_STREAM_CMD_STOP = 0x02,
    DEK_UART_STREAM_CMD_WRITE = 0x03,
    DEK_UART_STREAM_CMD_GET_STATUS = 0x04,
    DEK_UART_STREAM_CMD_FLUSH_RX = 0x05
} dek_uart_stream_command_t;

typedef enum
{
    DEK_UART_STREAM_STATE_STOPPED = 0,
    DEK_UART_STREAM_STATE_STARTING = 1,
    DEK_UART_STREAM_STATE_RUNNING = 2,
    DEK_UART_STREAM_STATE_STOPPING = 3,
    DEK_UART_STREAM_STATE_FAULTED = 4
} dek_uart_stream_service_state_t;

typedef enum
{
    DEK_UART_STREAM_EVENT_STATE_CHANGED = 0x01,
    DEK_UART_STREAM_EVENT_FAULT = 0x02
} dek_uart_stream_event_t;

typedef enum
{
    DEK_UART_STREAM_ERROR_UNSUPPORTED_BAUD = 0x01,
    DEK_UART_STREAM_ERROR_UNSUPPORTED_FRAMING = 0x02,
    DEK_UART_STREAM_ERROR_UNSUPPORTED_FLOW_CONTROL = 0x03,
    DEK_UART_STREAM_ERROR_INVALID_STATE = 0x04,
    DEK_UART_STREAM_ERROR_TX_BUFFER_BUSY = 0x05,
    DEK_UART_STREAM_ERROR_RX_BUFFER_FAULT = 0x06,
    DEK_UART_STREAM_ERROR_PROVIDER_FAULT = 0x07
} dek_uart_stream_error_t;

typedef enum
{
    DEK_UART_STREAM_LINE_STATUS_CARRIER_PRESENT = 0x01,
    DEK_UART_STREAM_LINE_STATUS_BREAK_DETECTED = 0x02,
    DEK_UART_STREAM_LINE_STATUS_RX_OVERFLOW = 0x04,
    DEK_UART_STREAM_LINE_STATUS_FRAMING_ERROR = 0x08,
    DEK_UART_STREAM_LINE_STATUS_PARITY_ERROR = 0x10
} dek_uart_stream_line_status_flag_t;

typedef enum
{
    DEK_UART_STREAM_STREAM_STATUS_RX_OVERFLOW = 0x01,
    DEK_UART_STREAM_STREAM_STATUS_FRAMING_ERROR = 0x02,
    DEK_UART_STREAM_STREAM_STATUS_PARITY_ERROR = 0x04,
    DEK_UART_STREAM_STREAM_STATUS_BREAK_OBSERVED = 0x08
} dek_uart_stream_stream_status_flag_t;

#pragma pack(push, 1)
typedef struct
{
    uint32_t baud_rate;
    uint8_t data_bits;
    uint8_t parity;
    uint8_t stop_bits;
    uint8_t flow_control;
    uint16_t rx_packet_size_hint;
    uint16_t reserved;
} dek_uart_stream_open_config_t;

typedef struct
{
    uint8_t service_state;
    uint8_t reserved0;
    uint16_t reserved1;
} dek_uart_stream_state_response_t;

typedef struct
{
    uint16_t tx_length;
} dek_uart_stream_write_request_header_t;

typedef struct
{
    uint16_t accepted_length;
    uint16_t reserved;
} dek_uart_stream_write_response_t;

typedef struct
{
    uint8_t service_state;
    uint8_t line_status_flags;
    uint16_t rx_overflow_count;
    uint16_t framing_error_count;
    uint16_t parity_error_count;
    uint16_t break_count;
    uint16_t reserved;
} dek_uart_stream_status_response_t;

typedef struct
{
    uint16_t cleared_bytes;
    uint16_t reserved;
} dek_uart_stream_flush_rx_response_t;

typedef struct
{
    uint32_t rx_sequence;
    uint8_t status_flags;
    uint8_t reserved0;
    uint16_t data_length;
} dek_uart_stream_stream_data_header_t;

typedef struct
{
    uint8_t old_state;
    uint8_t new_state;
    uint16_t reserved;
} dek_uart_stream_state_changed_event_t;

typedef struct
{
    uint8_t fault_code;
    uint8_t line_status_flags;
    uint16_t detail;
} dek_uart_stream_fault_event_t;
#pragma pack(pop)

void dek_uart_stream_open_config_init(dek_uart_stream_open_config_t *config);
bool dek_uart_stream_open_config_is_valid(const dek_uart_stream_open_config_t *config);

#endif
