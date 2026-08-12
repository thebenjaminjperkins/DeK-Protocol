#include "message-types/dek_service_command.h"

#include <string.h>

void dek_service_command_request_header_init(
    dek_service_command_request_header_t *header)
{
    if (header == NULL)
    {
        return;
    }

    memset(header, 0, sizeof(*header));
}

void dek_service_command_response_header_init(
    dek_service_command_response_header_t *header)
{
    if (header == NULL)
    {
        return;
    }

    memset(header, 0, sizeof(*header));
}

bool dek_service_command_status_is_valid(uint8_t status)
{
    return status <= (uint8_t)DEK_SERVICE_COMMAND_STATUS_PROVIDER_FAULT;
}

bool dek_service_command_request_header_is_valid(
    const dek_service_command_request_header_t *header)
{
    if (header == NULL)
    {
        return false;
    }

    return header->command_id != 0u &&
           header->reserved0 == 0u &&
           header->reserved1 == 0u;
}

bool dek_service_command_response_header_is_valid(
    const dek_service_command_response_header_t *header)
{
    if (header == NULL)
    {
        return false;
    }

    return header->command_id != 0u &&
           dek_service_command_status_is_valid(header->status) &&
           header->reserved == 0u;
}
