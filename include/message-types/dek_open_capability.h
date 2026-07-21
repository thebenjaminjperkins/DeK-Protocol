#ifndef DEK_OPEN_CAPABILITY_H
#define DEK_OPEN_CAPABILITY_H

#include <stdbool.h>
#include <stdint.h>

#define DEK_OPEN_CAPABILITY_FLAG_NONE 0u

#pragma pack(push, 1)
typedef struct
{
    uint8_t capability_id_length;
    uint16_t capability_version;
    uint16_t open_flags;
    uint16_t config_length;
} dek_open_capability_payload_header_t;

typedef struct
{
    uint16_t channel_id;
    uint16_t service_flags;
    uint16_t accepted_config_length;
} dek_open_ack_payload_header_t;

typedef struct
{
    uint16_t target_channel_id;
    uint16_t reserved;
} dek_close_capability_payload_t;

typedef enum
{
    DEK_ERROR_CLASS_NONE = 0,
    DEK_ERROR_CLASS_MALFORMED_PACKET = 1,
    DEK_ERROR_CLASS_UNSUPPORTED_PROTOCOL = 2,
    DEK_ERROR_CLASS_UNSUPPORTED_MESSAGE_TYPE = 3,
    DEK_ERROR_CLASS_INVALID_CHANNEL = 4,
    DEK_ERROR_CLASS_BUSY = 5,
    DEK_ERROR_CLASS_TIMEOUT = 6,
    DEK_ERROR_CLASS_INTERNAL_MODULE_FAULT = 7
} dek_error_class_t;

typedef struct
{
    uint8_t error_class;
    uint8_t error_code;
    uint16_t related_sequence;
    uint16_t related_channel;
    uint8_t detail_length;
} dek_error_payload_header_t;
#pragma pack(pop)

void dek_open_capability_payload_header_init(dek_open_capability_payload_header_t *payload);
void dek_open_ack_payload_header_init(dek_open_ack_payload_header_t *payload);
void dek_close_capability_payload_init(dek_close_capability_payload_t *payload);
void dek_error_payload_header_init(dek_error_payload_header_t *payload);
bool dek_error_class_is_valid(uint8_t error_class);
bool dek_open_ack_payload_header_is_valid(const dek_open_ack_payload_header_t *payload);

#endif
