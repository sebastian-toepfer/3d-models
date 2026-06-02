/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef I2C_H
#define I2C_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  struct I2C;
  typedef struct
  {
    const void *platform_info;
  } i2c_config_t;

  struct I2C *i2c_create(const i2c_config_t *config);
  bool i2c_write(struct I2C *i2c, uint8_t address, const uint8_t *data,
                 size_t len);
  bool i2c_read(struct I2C *i2c, uint8_t address, uint8_t *data, size_t len);
  bool i2c_start_stop(struct I2C *i2c, uint8_t address);
  void i2c_destroy(struct I2C *i2c);

#ifdef __cplusplus
}
#endif
#endif
