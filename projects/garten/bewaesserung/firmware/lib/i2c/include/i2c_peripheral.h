/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef I2C_PERIPHERAL_H
#define I2C_PERIPHERAL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  struct I2C;
  struct I2CPeripheral;

  typedef struct
  {
    struct I2C *i2c;
    uint8_t address;
  } i2c_peripheral_config_t;

  struct I2CPeripheral *
  i2c_peripheral_create(const i2c_peripheral_config_t *config);
  bool i2c_peripheral_write(struct I2CPeripheral *peripheral,
                            const uint8_t *data, size_t len);
  bool i2c_peripheral_read(struct I2CPeripheral *peripheral, uint8_t *data,
                           size_t len);
  bool i2c_peripheral_start_stop(struct I2CPeripheral *peripheral);
  bool i2c_peripheral_start_stop_at(struct I2CPeripheral *peripheral,
                                    uint8_t address);
  void i2c_peripheral_destroy(struct I2CPeripheral *peripheral);

#ifdef __cplusplus
}
#endif
#endif
