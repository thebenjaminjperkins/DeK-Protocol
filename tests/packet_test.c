#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <dek_protocol/dek_packet.h>
#include <dek_protocol/dek_receiver.h>
#include <dek_protocol/dek_transport.h>
#include <dek_protocol/message-types/dek_hello.h>

static void print_test_banner(const char *test_name)
{
    printf("\n[TEST] %s\n", test_name);
}

static void print_step(const char *test_name, const char *step_name)
{
    printf("  [%s] %s\n", test_name, step_name);
}

static bool require_true(
    const char *test_name,
    const char *step_name,
    bool condition,
    const char *failure_message)
{
    print_step(test_name, step_name);

    if (!condition)
    {
        fprintf(stderr, "    FAIL: %s\n", failure_message);
        return false;
    }

    printf("    OK\n");
    return true;
}

static bool test_packet_round_trip(void)
{
    static const char *test_name = "test_packet_round_trip";
    static const uint8_t payload[] = {0x10, 0x20, 0x30, 0x40};
    uint8_t buffer[64];
    dek_packet_t packet;
    dek_packet_t decoded;

    print_test_banner(test_name);

    print_step(test_name, "initialize packet header");
    dek_packet_init(&packet.header);
    packet.header.message_type = DEK_MSG_COMMAND;
    packet.header.flags = DEK_FLAG_RESPONSE_REQUIRED;
    packet.header.sequence_number = 7u;
    packet.header.channel_id = 3u;
    packet.header.payload_length = (uint16_t)sizeof(payload);
    packet.payload = payload;

    if (!require_true(
            test_name,
            "encode packet",
            dek_packet_encode(&packet, buffer, sizeof(buffer)),
            "dek_packet_encode should succeed"))
    {
        return false;
    }

    if (!require_true(
            test_name,
            "validate encoded packet",
            dek_packet_validate(buffer, dek_packet_encoded_size(sizeof(payload)), NULL),
            "dek_packet_validate should accept a freshly encoded packet"))
    {
        return false;
    }

    if (!require_true(
            test_name,
            "decode packet",
            dek_packet_decode(&decoded, buffer, dek_packet_encoded_size(sizeof(payload))),
            "dek_packet_decode should succeed for the encoded packet"))
    {
        return false;
    }

    if (!require_true(
            test_name,
            "verify decoded metadata",
            decoded.header.message_type == DEK_MSG_COMMAND &&
                decoded.header.flags == DEK_FLAG_RESPONSE_REQUIRED &&
                decoded.header.sequence_number == 7u &&
                decoded.header.channel_id == 3u &&
                decoded.header.payload_length == sizeof(payload),
            "decoded header fields should match the source packet"))
    {
        return false;
    }

    return require_true(
        test_name,
        "verify decoded payload bytes",
        memcmp(decoded.payload, payload, sizeof(payload)) == 0,
        "decoded payload bytes should match the source payload");
}

static bool test_packet_rejects_bad_crc(void)
{
    static const char *test_name = "test_packet_rejects_bad_crc";
    static const uint8_t payload[] = {0xAA, 0xBB};
    uint8_t buffer[64];
    dek_packet_t packet;
    uint16_t packet_size;

    print_test_banner(test_name);

    print_step(test_name, "initialize packet header");
    dek_packet_init(&packet.header);
    packet.header.message_type = DEK_MSG_PING;
    packet.header.payload_length = (uint16_t)sizeof(payload);
    packet.payload = payload;

    if (!require_true(
            test_name,
            "encode packet with valid crc",
            dek_packet_encode(&packet, buffer, sizeof(buffer)),
            "dek_packet_encode should succeed"))
    {
        return false;
    }

    packet_size = dek_packet_encoded_size(sizeof(payload));
    print_step(test_name, "corrupt crc byte");
    buffer[packet_size - 1u] ^= 0xFFu;

    return require_true(
        test_name,
        "validate corrupted packet",
        !dek_packet_validate(buffer, packet_size, NULL),
        "dek_packet_validate should reject a packet with a bad crc");
}

static bool test_hello_encode_decode(void)
{
    static const char *test_name = "test_hello_encode_decode";
    dek_hello_payload_t hello;
    dek_hello_payload_t decoded;
    uint8_t buffer[DEK_HELLO_PAYLOAD_SIZE];

    print_test_banner(test_name);

    print_step(test_name, "initialize hello payload");
    dek_hello_payload_init(&hello);
    hello.host_flags = 0x1234u;

    if (!require_true(
            test_name,
            "encode hello payload",
            dek_hello_encode(&hello, buffer, sizeof(buffer)),
            "dek_hello_encode should succeed"))
    {
        return false;
    }

    if (!require_true(
            test_name,
            "decode hello payload",
            dek_hello_decode(&decoded, buffer, sizeof(buffer)),
            "dek_hello_decode should succeed"))
    {
        return false;
    }

    return require_true(
        test_name,
        "verify decoded hello fields",
        decoded.min_protocol_version == DEK_PROTOCOL_VERSION &&
            decoded.max_protocol_version == DEK_PROTOCOL_VERSION &&
            decoded.host_flags == 0x1234u,
        "decoded hello payload should preserve protocol versions and host flags");
}

static bool test_transport_hello(void)
{
    static const char *test_name = "test_transport_hello";
    dek_transport_t transport;
    dek_packet_t packet;
    dek_hello_payload_t hello;
    uint8_t tx_buffer[64];
    uint16_t encoded_length = 0u;

    print_test_banner(test_name);

    print_step(test_name, "initialize transport state");
    dek_transport_init(&transport);

    if (!require_true(
            test_name,
            "verify transport counters start at zero",
            transport.packets_sent == 0u &&
                transport.packets_received == 0u &&
                transport.crc_errors == 0u,
            "transport counters should be zero after init"))
    {
        return false;
    }

    if (!require_true(
            test_name,
            "send hello packet",
            dek_transport_send_hello(&transport, tx_buffer, sizeof(tx_buffer), &encoded_length),
            "dek_transport_send_hello should succeed"))
    {
        return false;
    }

    if (!require_true(
            test_name,
            "verify encoded length is hello sized",
            encoded_length == dek_packet_encoded_size(DEK_HELLO_PAYLOAD_SIZE),
            "hello encoded length should match the packet helper"))
    {
        return false;
    }

    if (!require_true(
            test_name,
            "receive hello packet through transport",
            dek_transport_receive(&transport, &packet, tx_buffer, encoded_length),
            "dek_transport_receive should decode the hello packet"))
    {
        return false;
    }

    if (!require_true(
            test_name,
            "decode hello payload from received packet",
            dek_hello_decode(&hello, packet.payload, packet.header.payload_length),
            "dek_hello_decode should succeed on the received payload"))
    {
        return false;
    }

    if (!require_true(
            test_name,
            "verify transport counters",
            transport.packets_sent == 1u &&
                transport.packets_received == 1u,
            "transport should report one sent and one received packet"))
    {
        return false;
    }

    return require_true(
        test_name,
        "verify received hello packet fields",
        packet.header.message_type == DEK_MSG_HELLO &&
            packet.header.sequence_number == 1u &&
            packet.header.channel_id == 0u &&
            packet.header.payload_length == DEK_HELLO_PAYLOAD_SIZE &&
            hello.min_protocol_version == DEK_PROTOCOL_VERSION &&
            hello.max_protocol_version == DEK_PROTOCOL_VERSION &&
            hello.host_flags == 0u,
        "received hello packet should preserve the expected header and payload values");
}

static bool test_receiver_reassembles_packets(void)
{
    static const char *test_name = "test_receiver_reassembles_packets";
    dek_transport_t transport;
    dek_packet_t packet;
    dek_packet_receiver_t receiver;
    uint8_t receiver_buffer[64];
    uint8_t tx_buffer[64];
    uint16_t encoded_length = 0u;
    dek_receiver_feed_status_t status = DEK_RECEIVER_FEED_STATUS_SYNCING;

    print_test_banner(test_name);

    print_step(test_name, "initialize transport and receiver");
    dek_transport_init(&transport);
    dek_packet_receiver_init(&receiver, receiver_buffer, sizeof(receiver_buffer));

    if (!require_true(
            test_name,
            "build hello packet for receiver feed",
            dek_transport_send_hello(&transport, tx_buffer, sizeof(tx_buffer), &encoded_length),
            "dek_transport_send_hello should succeed"))
    {
        return false;
    }

    print_step(test_name, "feed packet bytes one at a time");
    for (uint16_t i = 0; i < encoded_length; ++i)
    {
        status = dek_packet_receiver_feed(&receiver, tx_buffer[i], &packet);

        if (i + 1u < encoded_length &&
            status == DEK_RECEIVER_FEED_STATUS_PACKET_READY)
        {
            fprintf(stderr, "    FAIL: receiver reported packet ready before final byte\n");
            return false;
        }
    }

    return require_true(
        test_name,
        "verify receiver completes on final byte",
        status == DEK_RECEIVER_FEED_STATUS_PACKET_READY &&
            packet.header.message_type == DEK_MSG_HELLO &&
            packet.header.payload_length == DEK_HELLO_PAYLOAD_SIZE,
        "receiver should report packet ready with a decoded hello packet");
}

static bool test_transport_tracks_crc_errors(void)
{
    static const char *test_name = "test_transport_tracks_crc_errors";
    dek_transport_t transport;
    dek_packet_t packet;
    uint8_t tx_buffer[64];
    uint16_t encoded_length = 0u;

    print_test_banner(test_name);

    print_step(test_name, "initialize transport");
    dek_transport_init(&transport);

    if (!require_true(
            test_name,
            "send hello packet",
            dek_transport_send_hello(&transport, tx_buffer, sizeof(tx_buffer), &encoded_length),
            "dek_transport_send_hello should succeed"))
    {
        return false;
    }

    print_step(test_name, "corrupt hello packet crc");
    tx_buffer[encoded_length - 1u] ^= 0x01u;

    if (!require_true(
            test_name,
            "reject corrupted packet during receive",
            !dek_transport_receive(&transport, &packet, tx_buffer, encoded_length),
            "dek_transport_receive should reject the corrupted packet"))
    {
        return false;
    }

    return require_true(
        test_name,
        "verify crc error accounting",
        transport.crc_errors == 1u &&
            transport.packets_received == 0u,
        "transport should count exactly one crc error and zero received packets");
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

    puts("\nAll protocol tests passed.");
    return 0;
}
