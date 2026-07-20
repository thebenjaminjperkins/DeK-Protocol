#include <string.h>

#include "dek_protocol/dek_crc.h"
#include "dek_protocol/dek_transport.h"
#include "dek_protocol/message-types/dek_hello.h"

static uint16_t dek_transport_allocate_sequence(dek_transport_t *transport)
{
    uint16_t sequence_number = transport->next_sequence_number++;

    if (transport->next_sequence_number == 0u)
    {
        transport->next_sequence_number = 1u;
    }

    return sequence_number;
}
/*
 * Reset the transport state before sending any packets.
 *
 * A starting sequence of 1 keeps the first packet distinguishable from an
 * uninitialized or zero-valued counter and preserves a simple monotonic order.
 */
void dek_transport_init(dek_transport_t *transport)
{
    if (transport == NULL)
    {
        return;
    }

    memset(transport, 0, sizeof(*transport));
    transport->next_sequence_number = 1u;
}

/*
 * Encode a packet using the transport-layer metadata.
 *
 * This function fills the packet header with the message type, logical channel,
 * payload length, and a freshly assigned sequence number. The payload bytes are
 * passed through without copying so the caller retains ownership of the data.
 */
bool dek_transport_send(
    dek_transport_t *transport,
    dek_message_type_t message_type,
    uint16_t channel,
    const uint8_t *payload,
    uint16_t payload_length,
    uint8_t *tx_buffer,
    uint16_t tx_buffer_size,
    uint16_t *encoded_length)
{
    if (transport == NULL || tx_buffer == NULL)
    {
        return false;
    }

    dek_packet_t packet;

    /* Initialize the header to the default protocol values before populating it. */
    dek_packet_init(&packet.header);

    /* Populate the packet header with transport-layer and message-specific fields. */
    packet.header.message_type = message_type;
    packet.header.sequence_number = dek_transport_allocate_sequence(transport);
    packet.header.channel_id = channel;
    packet.header.payload_length = payload_length;

    /* The payload is referenced directly by the encoder. */
    packet.payload = payload;

    if (!dek_packet_encode(
        &packet,
        tx_buffer,
        tx_buffer_size))
    {
        return false;
    }

    transport->packets_sent++;

    if (encoded_length != NULL)
    {
        *encoded_length = dek_packet_encoded_size(payload_length);
    }

    return true;
}

bool dek_transport_receive(
    dek_transport_t *transport,
    dek_packet_t *packet,
    const uint8_t *rx_buffer,
    uint16_t rx_buffer_size)
{
    dek_packet_header_t header;

    if (transport == NULL || packet == NULL || rx_buffer == NULL)
    {
        return false;
    }

    if (!dek_packet_decode_header(&header, rx_buffer, rx_buffer_size))
    {
        transport->malformed_packets++;
        return false;
    }

    {
        uint16_t expected_size = dek_packet_encoded_size(header.payload_length);

        if (rx_buffer_size < expected_size)
        {
            transport->malformed_packets++;
            return false;
        }

        {
            uint16_t crc_offset = (uint16_t)(expected_size - DEK_PACKET_CRC_SIZE);
            uint16_t expected_crc =
                (uint16_t)rx_buffer[crc_offset] |
                ((uint16_t)rx_buffer[crc_offset + 1] << 8);

            if (!dek_crc16_verify(rx_buffer, crc_offset, expected_crc))
            {
                transport->crc_errors++;
                return false;
            }
        }
    }

    if (!dek_packet_decode(packet, rx_buffer, rx_buffer_size))
    {
        transport->malformed_packets++;
        return false;
    }

    transport->packets_received++;
    return true;
}

bool dek_transport_send_hello(
    dek_transport_t *transport,
    uint8_t *tx_buffer,
    uint16_t tx_buffer_size,
    uint16_t *encoded_length)
{
    dek_hello_payload_t hello;
    uint8_t hello_buffer[DEK_HELLO_PAYLOAD_SIZE];

    dek_hello_payload_init(&hello);

    if (!dek_hello_encode(&hello, hello_buffer, sizeof(hello_buffer)))
    {
        return false;
    }

    return dek_transport_send(
        transport,
        DEK_MSG_HELLO,
        0u,
        hello_buffer,
        sizeof(hello_buffer),
        tx_buffer,
        tx_buffer_size,
        encoded_length);
}
