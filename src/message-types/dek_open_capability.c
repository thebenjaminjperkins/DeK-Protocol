#include "message-types/dek_open_capability.h"

#include <string.h>

void dek_open_capability_payload_header_init(dek_open_capability_payload_header_t *payload)
{
    if (payload == NULL)
    {
        return;
    }

    memset(payload, 0, sizeof(*payload));
}

void dek_open_ack_payload_header_init(dek_open_ack_payload_header_t *payload)
{
    if (payload == NULL)
    {
        return;
    }

    memset(payload, 0, sizeof(*payload));
}

void dek_close_capability_payload_init(dek_close_capability_payload_t *payload)
{
    if (payload == NULL)
    {
        return;
    }

    memset(payload, 0, sizeof(*payload));
}

void dek_error_payload_header_init(dek_error_payload_header_t *payload)
{
    if (payload == NULL)
    {
        return;
    }

    memset(payload, 0, sizeof(*payload));
}

bool dek_error_class_is_valid(uint8_t error_class)
{
    return error_class <= (uint8_t)DEK_ERROR_CLASS_INTERNAL_MODULE_FAULT;
}

bool dek_open_ack_payload_header_is_valid(const dek_open_ack_payload_header_t *payload)
{
    if (payload == NULL)
    {
        return false;
    }

    return payload->channel_id != 0u;
}
