/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "i2c_sercom.h"
#include "i2c.h"
#include <stddef.h>
#include <stdlib.h>

#define I2C_WRITE 0U
#define I2C_READ 1U

struct I2C
{
  Sercom *sercom;
  uint32_t timeout;
};

static bool i2c_wait_sync(const struct I2C *i2c)
{
  uint32_t timeout = i2c->timeout;
  while (i2c->sercom->I2CM.SYNCBUSY.reg != 0U)
  {
    if (timeout-- == 0U)
    {
      return false;
    }
  }
  return true;
}

static bool i2c_wait_master_on_bus(const struct I2C *i2c)
{
  uint32_t timeout = i2c->timeout;
  while (!i2c->sercom->I2CM.INTFLAG.bit.MB)
  {
    if (i2c->sercom->I2CM.STATUS.bit.BUSERR ||
        i2c->sercom->I2CM.STATUS.bit.ARBLOST || timeout-- == 0U)
    {
      return false;
    }
  }
  return !i2c->sercom->I2CM.STATUS.bit.RXNACK;
}

static bool i2c_wait_slave_on_bus(const struct I2C *i2c)
{
  uint32_t timeout = i2c->timeout;
  while (!i2c->sercom->I2CM.INTFLAG.bit.SB)
  {
    if (i2c->sercom->I2CM.INTFLAG.bit.MB ||
        i2c->sercom->I2CM.STATUS.bit.BUSERR ||
        i2c->sercom->I2CM.STATUS.bit.ARBLOST || timeout-- == 0U)
    {
      i2c->sercom->I2CM.CTRLB.bit.CMD = 3U;
      return false;
    }
  }
  return true;
}

static void i2c_stop(const struct I2C *i2c)
{
  i2c->sercom->I2CM.CTRLB.bit.CMD = 3U;
  i2c_wait_sync(i2c);
}

static bool i2c_start(const struct I2C *i2c, uint8_t address, uint8_t direction)
{
  i2c->sercom->I2CM.ADDR.bit.ADDR = (uint32_t)((address << 1U) | direction);

  if (direction == I2C_WRITE)
  {
    return i2c_wait_master_on_bus(i2c);
  }

  return i2c_wait_slave_on_bus(i2c);
}

static bool i2c_write_byte(const struct I2C *i2c, uint8_t data)
{
  i2c->sercom->I2CM.DATA.bit.DATA = data;
  return i2c_wait_master_on_bus(i2c);
}

static bool i2c_read_byte(const struct I2C *i2c, uint8_t *data, bool last_byte)
{
  uint32_t timeout = i2c->timeout;
  while (!i2c->sercom->I2CM.INTFLAG.bit.SB)
  {
    if (i2c->sercom->I2CM.INTFLAG.bit.MB ||
        i2c->sercom->I2CM.STATUS.bit.BUSERR ||
        i2c->sercom->I2CM.STATUS.bit.ARBLOST || timeout-- == 0U)
    {
      i2c_stop(i2c);
      return false;
    }
  }

  *data = (uint8_t)i2c->sercom->I2CM.DATA.bit.DATA;

  if (last_byte)
  {
    i2c->sercom->I2CM.CTRLB.bit.ACKACT = 1U;
    i2c_stop(i2c);
  }
  else
  {
    i2c->sercom->I2CM.CTRLB.bit.ACKACT = 0U;
    i2c->sercom->I2CM.CTRLB.bit.CMD = 2U;
    i2c_wait_sync(i2c);
  }

  return true;
}

static void configure_pin(PortGroup *group, uint8_t pin_index, uint8_t pin_mux)
{
  group->PINCFG[pin_index].bit.PMUXEN = 1U;
  if ((pin_index % 2U) == 0U)
  {
    group->PMUX[pin_index / 2U].bit.PMUXE = pin_mux;
  }
  else
  {
    group->PMUX[pin_index / 2U].bit.PMUXO = pin_mux;
  }
}

static void configure_bus(const I2CSercomInfo *info, struct I2C *i2c)
{
  PM->APBCMASK.reg |= info->peripheral_mask;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(info->core_clock_id) |
                      GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY)
  {
  }

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(info->slow_clock_id) |
                      GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY)
  {
  }

  configure_pin(info->sda_group, info->sda_pin_index, info->sda_pin_mux);
  configure_pin(info->scl_group, info->scl_pin_index, info->scl_pin_mux);

  i2c->sercom->I2CM.CTRLA.bit.SWRST = 1U;
  i2c_wait_sync(i2c);

  i2c->sercom->I2CM.CTRLA.reg =
      SERCOM_I2CM_CTRLA_MODE(SERCOM_I2CM_CTRLA_MODE_I2C_MASTER_Val);
  i2c->sercom->I2CM.BAUD.bit.BAUD =
      SystemCoreClock / (2U * info->baudrate) - 5U -
      (((SystemCoreClock / 1000000U) * info->rise_time_nanoseconds) /
       (2U * 1000U));

  i2c->sercom->I2CM.CTRLA.bit.ENABLE = 1U;
  i2c_wait_sync(i2c);

  i2c->sercom->I2CM.STATUS.bit.BUSSTATE = 1U;
  i2c_wait_sync(i2c);
}

struct I2C *i2c_create(const i2c_config_t *config)
{
  if (!config || !config->platform_info)
  {
    return NULL;
  }

  const I2CSercomInfo *info = (const I2CSercomInfo *)config->platform_info;
  if (!info->sercom || !info->sda_group || !info->scl_group ||
      info->baudrate == 0U || info->timeout == 0U)
  {
    return NULL;
  }

  struct I2C *result = malloc(sizeof(struct I2C));
  if (!result)
  {
    return NULL;
  }

  result->sercom = info->sercom;
  result->timeout = info->timeout;
  configure_bus(info, result);
  return result;
}

bool i2c_write(struct I2C *i2c, uint8_t address, const uint8_t *data,
               size_t len)
{
  if (!i2c || !data || len == 0U || !i2c_start(i2c, address, I2C_WRITE))
  {
    if (i2c)
    {
      i2c_stop(i2c);
    }
    return false;
  }

  for (size_t i = 0; i < len; i++)
  {
    if (!i2c_write_byte(i2c, data[i]))
    {
      i2c_stop(i2c);
      return false;
    }
  }

  i2c_stop(i2c);
  return true;
}

bool i2c_read(struct I2C *i2c, uint8_t address, uint8_t *data, size_t len)
{
  if (!i2c || !data || len == 0U || !i2c_start(i2c, address, I2C_READ))
  {
    if (i2c)
    {
      i2c_stop(i2c);
    }
    return false;
  }

  for (size_t i = 0; i < len; i++)
  {
    if (!i2c_read_byte(i2c, &data[i], i == (len - 1U)))
    {
      return false;
    }
  }

  return true;
}

bool i2c_start_stop(struct I2C *i2c, uint8_t address)
{
  if (!i2c)
  {
    return false;
  }

  bool started = i2c_start(i2c, address, I2C_WRITE);
  i2c_stop(i2c);
  return started;
}

void i2c_destroy(struct I2C *i2c)
{
  if (!i2c)
  {
    return;
  }
  free(i2c);
}
