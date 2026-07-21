#include "capabilities/dek_uart_stream.h"

#include <string.h>

void dek_uart_stream_open_config_init(dek_uart_stream_open_config_t *config)
{
    if (config == NULL)
    {
        return;
    }

    memset(config, 0, sizeof(*config));
}

bool dek_uart_stream_open_config_is_valid(const dek_uart_stream_open_config_t *config)
{
    if (config == NULL)
    {
        return false;
    }

    if (config->baud_rate == 0u ||
        config->data_bits < 5u ||
        config->data_bits > 8u ||
        config->parity > (uint8_t)DEK_UART_STREAM_PARITY_ODD ||
        (config->stop_bits != (uint8_t)DEK_UART_STREAM_STOP_BITS_1 &&
         config->stop_bits != (uint8_t)DEK_UART_STREAM_STOP_BITS_2) ||
        config->flow_control > (uint8_t)DEK_UART_STREAM_FLOW_CONTROL_RTS_CTS ||
        config->reserved != 0u)
    {
        return false;
    }

    return true;
}
