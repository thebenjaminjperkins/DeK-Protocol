#ifndef DEK_HELLO_H
#define DEK_HELLO_H

#include <stdbool.h>
#include <stdint.h>

#define DEK_HELLO_PAYLOAD_SIZE 4u

typedef struct
{
    uint8_t min_protocol_version;
    uint8_t max_protocol_version;
    uint16_t host_flags;
} dek_hello_payload_t;

void dek_hello_payload_init(dek_hello_payload_t *payload);
bool dek_hello_encode(
    const dek_hello_payload_t *payload,
    uint8_t *buffer,
    uint16_t buffer_size);
bool dek_hello_decode(
    dek_hello_payload_t *payload,
    const uint8_t *buffer,
    uint16_t buffer_size);

#endif
