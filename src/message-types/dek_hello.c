#include "dek_protocol/dek_message.h"
#include "dek_protocol/message-types/dek_hello.h"

#include <stddef.h>

void dek_hello_payload_init(dek_hello_payload_t *payload)
{
    if (payload == NULL)
    {
        return;
    }

    payload->min_protocol_version = DEK_PROTOCOL_VERSION;
    payload->max_protocol_version = DEK_PROTOCOL_VERSION;
    payload->host_flags = 0;
}

bool dek_hello_encode(
    const dek_hello_payload_t *payload,
    uint8_t *buffer,
    uint16_t buffer_size)
{
    if (payload == NULL || buffer == NULL || buffer_size < DEK_HELLO_PAYLOAD_SIZE)
    {
        return false;
    }

    buffer[0] = payload->min_protocol_version;
    buffer[1] = payload->max_protocol_version;
    buffer[2] = (uint8_t)(payload->host_flags & 0xFFu);
    buffer[3] = (uint8_t)((payload->host_flags >> 8) & 0xFFu);

    return true;
}

bool dek_hello_decode(
    dek_hello_payload_t *payload,
    const uint8_t *buffer,
    uint16_t buffer_size)
{
    if (payload == NULL || buffer == NULL || buffer_size < DEK_HELLO_PAYLOAD_SIZE)
    {
        return false;
    }

    payload->min_protocol_version = buffer[0];
    payload->max_protocol_version = buffer[1];
    payload->host_flags = (uint16_t)buffer[2] | ((uint16_t)buffer[3] << 8);

    return true;
}
