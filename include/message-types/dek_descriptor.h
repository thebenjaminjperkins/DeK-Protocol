#ifndef DEK_DESCRIPTOR_H
#define DEK_DESCRIPTOR_H

#include <stdbool.h>
#include <stdint.h>

#define DEK_MODULE_ID_SIZE 16u

#pragma pack(push, 1)
typedef struct
{
    uint8_t module_id[DEK_MODULE_ID_SIZE];
    uint16_t module_family_id;
    uint16_t module_model_id;
    uint8_t hardware_rev_major;
    uint8_t hardware_rev_minor;
    uint8_t firmware_rev_major;
    uint8_t firmware_rev_minor;
    uint8_t firmware_rev_patch;
    uint8_t supported_protocol_version;
    uint16_t max_payload_size;
    uint16_t capability_manifest_total_bytes;
    uint8_t capability_count;
    uint8_t health_flags;
} dek_descriptor_payload_t;
#pragma pack(pop)

void dek_descriptor_payload_init(dek_descriptor_payload_t *payload);
bool dek_module_id_is_valid(const uint8_t module_id[DEK_MODULE_ID_SIZE]);
bool dek_descriptor_payload_is_valid(const dek_descriptor_payload_t *payload);

#endif
