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

struct Transceiver *lora_create(const struct SecretStore *lora_secrets);

#ifdef __cplusplus
}
#endif
#endif