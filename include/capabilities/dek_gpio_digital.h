#ifndef DEK_GPIO_DIGITAL_H
#define DEK_GPIO_DIGITAL_H

#include <stdbool.h>
#include <stdint.h>

#include "capabilities/dek_capability_ids.h"

typedef enum
{
    DEK_GPIO_DIGITAL_CMD_SET_MODE = 0x01,
    DEK_GPIO_DIGITAL_CMD_WRITE = 0x02,
    DEK_GPIO_DIGITAL_CMD_READ = 0x03
} dek_gpio_digital_command_t;

typedef enum
{
    DEK_GPIO_DIGITAL_MODE_INPUT = 0,
    DEK_GPIO_DIGITAL_MODE_OUTPUT = 1,
    DEK_GPIO_DIGITAL_MODE_INPUT_PULLUP = 2,
    DEK_GPIO_DIGITAL_MODE_INPUT_PULLDOWN = 3
} dek_gpio_digital_pin_mode_t;

typedef enum
{
    DEK_GPIO_DIGITAL_LEVEL_LOW = 0,
    DEK_GPIO_DIGITAL_LEVEL_HIGH = 1
} dek_gpio_digital_level_t;

#pragma pack(push, 1)
typedef struct
{
    uint8_t pin;
    uint8_t mode;
    uint16_t reserved;
} dek_gpio_digital_set_mode_request_t;

typedef struct
{
    uint8_t pin;
    uint8_t applied_mode;
    uint16_t reserved;
} dek_gpio_digital_set_mode_response_t;

typedef struct
{
    uint8_t pin;
    uint8_t value;
    uint16_t reserved;
} dek_gpio_digital_write_request_t;

typedef struct
{
    uint8_t pin;
    uint8_t applied_value;
    uint16_t reserved;
} dek_gpio_digital_write_response_t;

typedef struct
{
    uint8_t pin;
    uint8_t reserved0;
    uint16_t reserved1;
} dek_gpio_digital_read_request_t;

typedef struct
{
    uint8_t pin;
    uint8_t value;
    uint16_t reserved;
} dek_gpio_digital_read_response_t;
#pragma pack(pop)

bool dek_gpio_digital_pin_mode_is_valid(uint8_t mode);
bool dek_gpio_digital_level_is_valid(uint8_t value);
bool dek_gpio_digital_set_mode_request_is_valid(const dek_gpio_digital_set_mode_request_t *request);
bool dek_gpio_digital_write_request_is_valid(const dek_gpio_digital_write_request_t *request);
bool dek_gpio_digital_read_request_is_valid(const dek_gpio_digital_read_request_t *request);

#endif
