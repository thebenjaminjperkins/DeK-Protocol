#include "dek_protocol/dek_receiver.h"

void dek_packet_receiver_init(
    dek_packet_receiver_t *receiver,
    uint8_t *buffer,
    uint16_t buffer_capacity)
{
    if (receiver == NULL)
    {
        return;
    }

    receiver->buffer = buffer;
    receiver->buffer_capacity = buffer_capacity;
    receiver->length = 0u;
    receiver->expected_length = 0u;
}

void dek_packet_receiver_reset(dek_packet_receiver_t *receiver)
{
    if (receiver == NULL)
    {
        return;
    }

    receiver->length = 0u;
    receiver->expected_length = 0u;
}

dek_receiver_feed_status_t dek_packet_receiver_feed(
    dek_packet_receiver_t *receiver,
    uint8_t byte,
    dek_packet_t *packet)
{
    if (receiver == NULL || receiver->buffer == NULL || packet == NULL)
    {
        return DEK_RECEIVER_FEED_STATUS_INVALID_PACKET;
    }

    if (receiver->length == 0u)
    {
        if (byte != DEK_PACKET_MAGIC_BYTE0)
        {
            return DEK_RECEIVER_FEED_STATUS_SYNCING;
        }
    }
    else if (receiver->length == 1u)
    {
        if (byte != DEK_PACKET_MAGIC_BYTE1)
        {
            receiver->buffer[0] = byte;
            receiver->length = (byte == DEK_PACKET_MAGIC_BYTE0) ? 1u : 0u;
            return DEK_RECEIVER_FEED_STATUS_SYNCING;
        }
    }

    if (receiver->length >= receiver->buffer_capacity)
    {
        dek_packet_receiver_reset(receiver);
        return DEK_RECEIVER_FEED_STATUS_BUFFER_OVERFLOW;
    }

    receiver->buffer[receiver->length++] = byte;

    if (receiver->length < DEK_PACKET_HEADER_SIZE)
    {
        return DEK_RECEIVER_FEED_STATUS_IN_PROGRESS;
    }

    if (receiver->expected_length == 0u)
    {
        dek_packet_header_t header;

        if (!dek_packet_decode_header(&header, receiver->buffer, receiver->length))
        {
            dek_packet_receiver_reset(receiver);
            return DEK_RECEIVER_FEED_STATUS_INVALID_PACKET;
        }

        receiver->expected_length = dek_packet_encoded_size(header.payload_length);

        if (receiver->expected_length > receiver->buffer_capacity)
        {
            dek_packet_receiver_reset(receiver);
            return DEK_RECEIVER_FEED_STATUS_BUFFER_OVERFLOW;
        }
    }

    if (receiver->length < receiver->expected_length)
    {
        return DEK_RECEIVER_FEED_STATUS_IN_PROGRESS;
    }

    if (!dek_packet_decode(packet, receiver->buffer, receiver->expected_length))
    {
        dek_packet_receiver_reset(receiver);
        return DEK_RECEIVER_FEED_STATUS_INVALID_PACKET;
    }

    dek_packet_receiver_reset(receiver);
    return DEK_RECEIVER_FEED_STATUS_PACKET_READY;
}
