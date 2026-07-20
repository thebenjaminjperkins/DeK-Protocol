#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <dek_protocol/dek_packet.h>
#include <dek_protocol/dek_receiver.h>
#include <dek_protocol/dek_transport.h>
#include <dek_protocol/message-types/dek_hello.h>

static bool test_packet_round_trip(void)
{
    static const uint8_t payload[] = {0x10, 0x20, 0x30, 0x40};
    uint8_t buffer[64];
    dek_packet_t packet;
    dek_packet_t decoded;

    dek_packet_init(&packet.header);
    packet.header.message_type = DEK_MSG_COMMAND;
    packet.header.flags = DEK_FLAG_RESPONSE_REQUIRED;
    packet.header.sequence_number = 7u;
    packet.header.channel_id = 3u;
    packet.header.payload_length = (uint16_t)sizeof(payload);
    packet.payload = payload;

    if (!dek_packet_encode(&packet, buffer, sizeof(buffer)))
    {
        return false;
    }

    if (!dek_packet_validate(buffer, dek_packet_encoded_size(sizeof(payload)), NULL))
    {
        return false;
    }

    if (!dek_packet_decode(&decoded, buffer, dek_packet_encoded_size(sizeof(payload))))
    {
        return false;
    }

    return decoded.header.message_type == DEK_MSG_COMMAND &&
           decoded.header.flags == DEK_FLAG_RESPONSE_REQUIRED &&
           decoded.header.sequence_number == 7u &&
           decoded.header.channel_id == 3u &&
           decoded.header.payload_length == sizeof(payload) &&
           memcmp(decoded.payload, payload, sizeof(payload)) == 0;
}

static bool test_packet_rejects_bad_crc(void)
{
    static const uint8_t payload[] = {0xAA, 0xBB};
    uint8_t buffer[64];
    dek_packet_t packet;
    uint16_t packet_size;

    dek_packet_init(&packet.header);
    packet.header.message_type = DEK_MSG_PING;
    packet.header.payload_length = (uint16_t)sizeof(payload);
    packet.payload = payload;

    if (!dek_packet_encode(&packet, buffer, sizeof(buffer)))
    {
        return false;
    }

    packet_size = dek_packet_encoded_size(sizeof(payload));
    buffer[packet_size - 1u] ^= 0xFFu;

    return !dek_packet_validate(buffer, packet_size, NULL);
}

static bool test_hello_encode_decode(void)
{
    dek_hello_payload_t hello;
    dek_hello_payload_t decoded;
    uint8_t buffer[DEK_HELLO_PAYLOAD_SIZE];

    dek_hello_payload_init(&hello);
    hello.host_flags = 0x1234u;

    if (!dek_hello_encode(&hello, buffer, sizeof(buffer)))
    {
        return false;
    }

    if (!dek_hello_decode(&decoded, buffer, sizeof(buffer)))
    {
        return false;
    }

    return decoded.min_protocol_version == DEK_PROTOCOL_VERSION &&
           decoded.max_protocol_version == DEK_PROTOCOL_VERSION &&
           decoded.host_flags == 0x1234u;
}

static bool test_transport_hello(void)
{
    dek_transport_t transport;
    dek_packet_t packet;
    dek_hello_payload_t hello;
    uint8_t tx_buffer[64];
    uint16_t encoded_length = 0u;

    dek_transport_init(&transport);

    if (!dek_transport_send_hello(&transport, tx_buffer, sizeof(tx_buffer), &encoded_length))
    {
        return false;
    }

    if (!dek_transport_receive(&transport, &packet, tx_buffer, encoded_length))
    {
        return false;
    }

    if (!dek_hello_decode(&hello, packet.payload, packet.header.payload_length))
    {
        return false;
    }

    return transport.packets_sent == 1u &&
           transport.packets_received == 1u &&
           packet.header.message_type == DEK_MSG_HELLO &&
           packet.header.sequence_number == 1u &&
           packet.header.channel_id == 0u &&
           packet.header.payload_length == DEK_HELLO_PAYLOAD_SIZE &&
           hello.min_protocol_version == DEK_PROTOCOL_VERSION &&
           hello.max_protocol_version == DEK_PROTOCOL_VERSION &&
           hello.host_flags == 0u;
}

static bool test_receiver_reassembles_packets(void)
{
    dek_transport_t transport;
    dek_packet_t packet;
    dek_packet_receiver_t receiver;
    uint8_t receiver_buffer[64];
    uint8_t tx_buffer[64];
    uint16_t encoded_length = 0u;
    dek_receiver_feed_status_t status = DEK_RECEIVER_FEED_STATUS_SYNCING;

    dek_transport_init(&transport);
    dek_packet_receiver_init(&receiver, receiver_buffer, sizeof(receiver_buffer));

    if (!dek_transport_send_hello(&transport, tx_buffer, sizeof(tx_buffer), &encoded_length))
    {
        return false;
    }

    for (uint16_t i = 0; i < encoded_length; ++i)
    {
        status = dek_packet_receiver_feed(&receiver, tx_buffer[i], &packet);
    }

    return status == DEK_RECEIVER_FEED_STATUS_PACKET_READY &&
           packet.header.message_type == DEK_MSG_HELLO &&
           packet.header.payload_length == DEK_HELLO_PAYLOAD_SIZE;
}

static bool test_transport_tracks_crc_errors(void)
{
    dek_transport_t transport;
    dek_packet_t packet;
    uint8_t tx_buffer[64];
    uint16_t encoded_length = 0u;

    dek_transport_init(&transport);

    if (!dek_transport_send_hello(&transport, tx_buffer, sizeof(tx_buffer), &encoded_length))
    {
        return false;
    }

    tx_buffer[encoded_length - 1u] ^= 0x01u;

    if (dek_transport_receive(&transport, &packet, tx_buffer, encoded_length))
    {
        return false;
    }

    return transport.crc_errors == 1u &&
           transport.packets_received == 0u;
}

int main(void)
{
    if (!test_packet_round_trip())
    {
        fputs("test_packet_round_trip failed\n", stderr);
        return 1;
    }

    if (!test_packet_rejects_bad_crc())
    {
        fputs("test_packet_rejects_bad_crc failed\n", stderr);
        return 1;
    }

    if (!test_hello_encode_decode())
    {
        fputs("test_hello_encode_decode failed\n", stderr);
        return 1;
    }

    if (!test_transport_hello())
    {
        fputs("test_transport_hello failed\n", stderr);
        return 1;
    }

    if (!test_receiver_reassembles_packets())
    {
        fputs("test_receiver_reassembles_packets failed\n", stderr);
        return 1;
    }

    if (!test_transport_tracks_crc_errors())
    {
        fputs("test_transport_tracks_crc_errors failed\n", stderr);
        return 1;
    }

    puts("All protocol tests passed.");
    return 0;
}
