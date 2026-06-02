/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#ifndef ECCX08_H
#define ECCX08_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "i2c_peripheral.h"

#define ECCX08_I2C_ADDRESS 0x60U
#define ECCX08_MAX_SLOT 15U

#ifdef __cplusplus
extern "C"
{
#endif

  struct ECCX08;

  typedef struct
  {
    struct I2CPeripheral *i2c;
  } eccx08_config_t;

  struct ECCX08 *eccx08_create(const eccx08_config_t *config);
  bool eccx08_begin(struct ECCX08 *eccx08);
  bool eccx08_check_access(struct ECCX08 *eccx08);
  bool eccx08_read_slot(struct ECCX08 *eccx08, uint8_t slot, uint8_t *data,
                        size_t len);
  bool eccx08_write_slot(struct ECCX08 *eccx08, uint8_t slot,
                         const uint8_t *data, size_t len);
  void eccx08_destroy(struct ECCX08 *eccx08);

#ifdef __cplusplus
}
#endif
#endif
