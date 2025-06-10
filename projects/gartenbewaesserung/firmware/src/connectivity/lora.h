#ifndef LORA_H
#define LORA_H
#include <stddef.h>
#include <stdint.h>

#include "transceiver.h"
#include "secretstore.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct LoRa;
typedef void (*lora_data_handler_t)(uint8_t *data, size_t len, void *user_data);

struct LoRa *lora_create(const struct SecretStore *lora_secrets);
struct Transceiver *lora_transceiver(const struct LoRa *lora);
void lora_poll(const struct LoRa *lora);
void lora_register_handler(struct LoRa *lora, uint8_t fport, const lora_data_handler_t handler, void *user_data);

#ifdef __cplusplus
}
#endif
#endif