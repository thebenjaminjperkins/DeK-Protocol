#ifndef DEK_RECEIVER_H
#define DEK_RECEIVER_H

#include <stdint.h>

#include "dek_protocol/dek_packet.h"

typedef enum
{
    DEK_RECEIVER_FEED_STATUS_SYNCING = 0,
    DEK_RECEIVER_FEED_STATUS_IN_PROGRESS,
    DEK_RECEIVER_FEED_STATUS_PACKET_READY,
    DEK_RECEIVER_FEED_STATUS_INVALID_PACKET,
    DEK_RECEIVER_FEED_STATUS_BUFFER_OVERFLOW
} dek_receiver_feed_status_t;

typedef struct
{
    uint8_t *buffer;
    uint16_t buffer_capacity;
    uint16_t length;
    uint16_t expected_length;
} dek_packet_receiver_t;

void dek_packet_receiver_init(
    dek_packet_receiver_t *receiver,
    uint8_t *buffer,
    uint16_t buffer_capacity);

void dek_packet_receiver_reset(dek_packet_receiver_t *receiver);

dek_receiver_feed_status_t dek_packet_receiver_feed(
    dek_packet_receiver_t *receiver,
    uint8_t byte,
    dek_packet_t *packet);

#endif
