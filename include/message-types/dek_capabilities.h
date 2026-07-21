#ifndef DEK_CAPABILITIES_H
#define DEK_CAPABILITIES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DEK_CAPABILITY_ID_MAX_LENGTH 32u

typedef enum
{
    DEK_RESOURCE_POLICY_SHARED = 0,
    DEK_RESOURCE_POLICY_EXCLUSIVE = 1,
    DEK_RESOURCE_POLICY_RESERVABLE = 2
} dek_resource_policy_t;

#pragma pack(push, 1)
typedef struct
{
    uint16_t manifest_offset;
    uint16_t requested_length;
} dek_get_capabilities_payload_t;

typedef struct
{
    uint16_t manifest_total_bytes;
    uint16_t chunk_offset;
    uint16_t chunk_length;
} dek_capabilities_payload_header_t;

typedef struct
{
    uint8_t capability_id_length;
    uint16_t capability_version;
    uint8_t resource_policy;
    uint8_t command_count;
    uint8_t event_count;
    uint16_t capability_flags;
    uint16_t limits_blob_length;
} dek_capability_manifest_entry_header_t;
#pragma pack(pop)

void dek_get_capabilities_payload_init(dek_get_capabilities_payload_t *payload);
void dek_capabilities_payload_header_init(dek_capabilities_payload_header_t *payload);
void dek_capability_manifest_entry_header_init(dek_capability_manifest_entry_header_t *entry);
bool dek_resource_policy_is_valid(uint8_t resource_policy);
bool dek_capability_id_is_valid(const char *capability_id);
bool dek_capability_manifest_entry_header_is_valid(
    const dek_capability_manifest_entry_header_t *entry,
    size_t encoded_length);

#endif
