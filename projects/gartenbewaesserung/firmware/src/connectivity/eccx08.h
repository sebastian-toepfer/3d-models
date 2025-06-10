#ifndef ECCX08_H
#define ECCX08_H
#include <stdint.h>

#include "secretstore.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct SecretStore *eccx08_create(uint8_t slot);

#ifdef __cplusplus
}
#endif

#endif