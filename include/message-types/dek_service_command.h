#ifndef DEK_SERVICE_COMMAND_H
#define DEK_SERVICE_COMMAND_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    DEK_SERVICE_COMMAND_STATUS_OK = 0,
    DEK_SERVICE_COMMAND_STATUS_UNSUPPORTED_COMMAND = 1,
    DEK_SERVICE_COMMAND_STATUS_INVALID_ARGUMENT = 2,
    DEK_SERVICE_COMMAND_STATUS_INVALID_STATE = 3,
    DEK_SERVICE_COMMAND_STATUS_PROVIDER_FAULT = 4
} dek_service_command_status_t;

#pragma pack(push, 1)
typedef struct
{
    uint8_t command_id;
    uint8_t reserved0;
    uint16_t reserved1;
} dek_service_command_request_header_t;

typedef struct
{
    uint8_t command_id;
    uint8_t status;
    uint16_t reserved;
} dek_service_command_response_header_t;
#pragma pack(pop)

void dek_service_command_request_header_init(
    dek_service_command_request_header_t *header);
void dek_service_command_response_header_init(
    dek_service_command_response_header_t *header);
bool dek_service_command_status_is_valid(uint8_t status);
bool dek_service_command_request_header_is_valid(
    const dek_service_command_request_header_t *header);
bool dek_service_command_response_header_is_valid(
    const dek_service_command_response_header_t *header);

#endif
