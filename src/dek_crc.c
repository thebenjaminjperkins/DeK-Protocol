#include "dek_protocol/dek_crc.h"

uint16_t dek_crc16(const uint8_t *data, uint16_t length)
{
    uint16_t crc = DEK_CRC16_INITIAL_VALUE;

    if (data == NULL || length == 0)
    {
        return crc;
    }

    for (uint16_t i = 0; i < length; ++i)
    {
        crc ^= ((uint16_t)data[i] << 8);

        for (int bit = 0; bit < 8; ++bit)
        {
            if ((crc & DEK_CRC16_MSB_MASK) != 0u)
            {
                crc = (uint16_t)((crc << 1) ^ DEK_CRC16_POLYNOMIAL);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

bool dek_crc16_verify(
    const uint8_t *data,
    uint16_t length,
    uint16_t expected_crc)
{
    return dek_crc16(data, length) == expected_crc;
}
