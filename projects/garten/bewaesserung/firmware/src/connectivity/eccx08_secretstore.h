/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef ECCX08_SECRETSTORE_H
#define ECCX08_SECRETSTORE_H
#include <stdint.h>

#include "eccx08.h"
#include "secretstore.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct SecretStore *eccx08_secretstore_create(struct ECCX08 *eccx08,
                                                uint8_t slot);

#ifdef __cplusplus
}
#endif
#endif
