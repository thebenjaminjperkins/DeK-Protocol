#include "message-types/dek_capabilities.h"

#include <string.h>

static bool dek_capability_id_char_is_valid(char value)
{
    if ((value >= 'a' && value <= 'z') ||
        (value >= 'A' && value <= 'Z') ||
        (value >= '0' && value <= '9'))
    {
        return true;
    }

    return value == '.' || value == '_' || value == '-';
}

void dek_get_capabilities_payload_init(dek_get_capabilities_payload_t *payload)
{
    if (payload == NULL)
    {
        return;
    }

    memset(payload, 0, sizeof(*payload));
}

void dek_capabilities_payload_header_init(dek_capabilities_payload_header_t *payload)
{
    if (payload == NULL)
    {
        return;
    }

    memset(payload, 0, sizeof(*payload));
}

void dek_capability_manifest_entry_header_init(dek_capability_manifest_entry_header_t *entry)
{
    if (entry == NULL)
    {
        return;
    }

    memset(entry, 0, sizeof(*entry));
}

bool dek_resource_policy_is_valid(uint8_t resource_policy)
{
    return resource_policy <= (uint8_t)DEK_RESOURCE_POLICY_RESERVABLE;
}

bool dek_capability_id_is_valid(const char *capability_id)
{
    size_t i;

    if (capability_id == NULL || capability_id[0] == '\0')
    {
        return false;
    }

    for (i = 0; capability_id[i] != '\0'; ++i)
    {
        if (i >= DEK_CAPABILITY_ID_MAX_LENGTH ||
            !dek_capability_id_char_is_valid(capability_id[i]))
        {
            return false;
        }
    }

    return true;
}

bool dek_capability_manifest_entry_header_is_valid(
    const dek_capability_manifest_entry_header_t *entry,
    size_t encoded_length)
{
    if (entry == NULL)
    {
        return false;
    }

    if (entry->capability_id_length == 0u ||
        entry->capability_id_length > DEK_CAPABILITY_ID_MAX_LENGTH ||
        entry->capability_version == 0u ||
        !dek_resource_policy_is_valid(entry->resource_policy))
    {
        return false;
    }

    return encoded_length >= sizeof(*entry) + (size_t)entry->capability_id_length;
}
