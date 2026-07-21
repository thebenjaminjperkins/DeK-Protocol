#include "message-types/dek_descriptor.h"

#include <string.h>

#include "dek_message.h"

void dek_descriptor_payload_init(dek_descriptor_payload_t *payload)
{
    if (payload == NULL)
    {
        return;
    }

    memset(payload, 0, sizeof(*payload));
    payload->supported_protocol_version = DEK_PROTOCOL_VERSION;
}

bool dek_module_id_is_valid(const uint8_t module_id[DEK_MODULE_ID_SIZE])
{
    size_t i;

    if (module_id == NULL)
    {
        return false;
    }

    for (i = 0; i < DEK_MODULE_ID_SIZE; ++i)
    {
        if (module_id[i] != 0u)
        {
            return true;
        }
    }

    return false;
}

bool dek_descriptor_payload_is_valid(const dek_descriptor_payload_t *payload)
{
    if (payload == NULL)
    {
        return false;
    }

    if (!dek_module_id_is_valid(payload->module_id))
    {
        return false;
    }

    if (payload->supported_protocol_version == 0u ||
        payload->max_payload_size == 0u)
    {
        return false;
    }

    return true;
}
