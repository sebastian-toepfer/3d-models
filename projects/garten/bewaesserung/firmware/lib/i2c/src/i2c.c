/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "i2c.h"
#include "i2c_peripheral.h"
#include <stddef.h>
#include <stdlib.h>

struct I2CPeripheral
{
  struct I2C *i2c;
  uint8_t address;
};

struct I2CPeripheral *
i2c_peripheral_create(const i2c_peripheral_config_t *config)
{
  if (!config || !config->i2c || config->address == 0U)
  {
    return NULL;
  }

  struct I2CPeripheral *result = malloc(sizeof(struct I2CPeripheral));
  if (!result)
  {
    return NULL;
  }

  result->i2c = config->i2c;
  result->address = config->address;
  return result;
}

bool i2c_peripheral_write(struct I2CPeripheral *peripheral, const uint8_t *data,
                          size_t len)
{
  if (!peripheral)
  {
    return false;
  }
  return i2c_write(peripheral->i2c, peripheral->address, data, len);
}

bool i2c_peripheral_read(struct I2CPeripheral *peripheral, uint8_t *data,
                         size_t len)
{
  if (!peripheral)
  {
    return false;
  }
  return i2c_read(peripheral->i2c, peripheral->address, data, len);
}

bool i2c_peripheral_start_stop(struct I2CPeripheral *peripheral)
{
  if (!peripheral)
  {
    return false;
  }
  return i2c_start_stop(peripheral->i2c, peripheral->address);
}

bool i2c_peripheral_start_stop_at(struct I2CPeripheral *peripheral,
                                  uint8_t address)
{
  if (!peripheral)
  {
    return false;
  }
  return i2c_start_stop(peripheral->i2c, address);
}

void i2c_peripheral_destroy(struct I2CPeripheral *peripheral)
{
  if (!peripheral)
  {
    return;
  }
  free(peripheral);
}
