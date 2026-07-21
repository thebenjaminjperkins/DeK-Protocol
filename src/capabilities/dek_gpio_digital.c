#include "capabilities/dek_gpio_digital.h"

bool dek_gpio_digital_pin_mode_is_valid(uint8_t mode)
{
    return mode <= (uint8_t)DEK_GPIO_DIGITAL_MODE_INPUT_PULLDOWN;
}

bool dek_gpio_digital_level_is_valid(uint8_t value)
{
    return value <= (uint8_t)DEK_GPIO_DIGITAL_LEVEL_HIGH;
}

bool dek_gpio_digital_set_mode_request_is_valid(const dek_gpio_digital_set_mode_request_t *request)
{
    if (request == NULL)
    {
        return false;
    }

    return dek_gpio_digital_pin_mode_is_valid(request->mode) &&
           request->reserved == 0u;
}

bool dek_gpio_digital_write_request_is_valid(const dek_gpio_digital_write_request_t *request)
{
    if (request == NULL)
    {
        return false;
    }

    return dek_gpio_digital_level_is_valid(request->value) &&
           request->reserved == 0u;
}

bool dek_gpio_digital_read_request_is_valid(const dek_gpio_digital_read_request_t *request)
{
    if (request == NULL)
    {
        return false;
    }

    return request->reserved0 == 0u &&
           request->reserved1 == 0u;
}
