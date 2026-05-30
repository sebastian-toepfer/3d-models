/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "eccx08.h"

#include "sam.h"
#include "secretstore.h"
#include "time/delay.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define ECCX08_I2C_ADDRESS 0x60U
#define ECCX08_MAX_SLOT 15U
#define ECCX08_WORD_SIZE 4U
#define ECCX08_BLOCK_SIZE 32U

#define ECCX08_WORD_ADDRESS_SLEEP 0x01U
#define ECCX08_WORD_ADDRESS_IDLE 0x02U
#define ECCX08_WORD_ADDRESS_COMMAND 0x03U

#define ECCX08_OPCODE_READ 0x02U
#define ECCX08_OPCODE_WRITE 0x12U
#define ECCX08_OPCODE_LOCK 0x17U
#define ECCX08_OPCODE_INFO 0x30U

#define ECCX08_ZONE_CONFIG 0U
#define ECCX08_ZONE_DATA 2U
#define ECCX08_ZONE_32_BYTES 0x80U

#define ECCX08_VERSION_MASK 0x0F00000UL
#define ECCX08_508_VERSION 0x0500000UL
#define ECCX08_608_VERSION 0x0600000UL

#define I2C_WRITE 0U
#define I2C_READ 1U
#define I2C_TIMEOUT 100000UL
#define I2C_BAUDRATE 100000UL
#define WIRE_RISE_TIME_NANOSECONDS 125UL

struct SecretStore
{
  uint8_t slot;
};

static bool i2c_wait_sync(void)
{
  uint32_t timeout = I2C_TIMEOUT;
  while (SERCOM2->I2CM.SYNCBUSY.reg != 0U)
  {
    if (timeout-- == 0U)
    {
      return false;
    }
  }
  return true;
}

static bool i2c_wait_master_on_bus(void)
{
  uint32_t timeout = I2C_TIMEOUT;
  while (!SERCOM2->I2CM.INTFLAG.bit.MB)
  {
    if (SERCOM2->I2CM.STATUS.bit.BUSERR || SERCOM2->I2CM.STATUS.bit.ARBLOST ||
        timeout-- == 0U)
    {
      return false;
    }
  }
  return !SERCOM2->I2CM.STATUS.bit.RXNACK;
}

static bool i2c_wait_slave_on_bus(void)
{
  uint32_t timeout = I2C_TIMEOUT;
  while (!SERCOM2->I2CM.INTFLAG.bit.SB)
  {
    if (SERCOM2->I2CM.INTFLAG.bit.MB || SERCOM2->I2CM.STATUS.bit.BUSERR ||
        SERCOM2->I2CM.STATUS.bit.ARBLOST || timeout-- == 0U)
    {
      SERCOM2->I2CM.CTRLB.bit.CMD = 3U;
      return false;
    }
  }
  return true;
}

static void i2c_stop(void)
{
  SERCOM2->I2CM.CTRLB.bit.CMD = 3U;
  i2c_wait_sync();
}

static bool i2c_start(uint8_t address, uint8_t direction)
{
  SERCOM2->I2CM.ADDR.bit.ADDR = (uint32_t)((address << 1U) | direction);

  if (direction == I2C_WRITE)
  {
    return i2c_wait_master_on_bus();
  }

  return i2c_wait_slave_on_bus();
}

static bool i2c_write_byte(uint8_t data)
{
  SERCOM2->I2CM.DATA.bit.DATA = data;
  return i2c_wait_master_on_bus();
}

static bool i2c_read_byte(uint8_t *data, bool last_byte)
{
  uint32_t timeout = I2C_TIMEOUT;
  while (!SERCOM2->I2CM.INTFLAG.bit.SB)
  {
    if (SERCOM2->I2CM.INTFLAG.bit.MB || SERCOM2->I2CM.STATUS.bit.BUSERR ||
        SERCOM2->I2CM.STATUS.bit.ARBLOST || timeout-- == 0U)
    {
      i2c_stop();
      return false;
    }
  }

  *data = (uint8_t)SERCOM2->I2CM.DATA.bit.DATA;

  if (last_byte)
  {
    SERCOM2->I2CM.CTRLB.bit.ACKACT = 1U;
    i2c_stop();
  }
  else
  {
    SERCOM2->I2CM.CTRLB.bit.ACKACT = 0U;
    SERCOM2->I2CM.CTRLB.bit.CMD = 2U;
    i2c_wait_sync();
  }

  return true;
}

static bool i2c_write(const uint8_t *data, size_t len)
{
  if (!data || len == 0U || !i2c_start(ECCX08_I2C_ADDRESS, I2C_WRITE))
  {
    i2c_stop();
    return false;
  }

  for (size_t i = 0; i < len; i++)
  {
    if (!i2c_write_byte(data[i]))
    {
      i2c_stop();
      return false;
    }
  }

  i2c_stop();
  return true;
}

static bool i2c_read(uint8_t *data, size_t len)
{
  if (!data || len == 0U || !i2c_start(ECCX08_I2C_ADDRESS, I2C_READ))
  {
    i2c_stop();
    return false;
  }

  for (size_t i = 0; i < len; i++)
  {
    if (!i2c_read_byte(&data[i], i == (len - 1U)))
    {
      return false;
    }
  }

  return true;
}

static void i2c_init(void)
{
  static bool initialized = false;
  if (initialized)
  {
    return;
  }

  PM->APBCMASK.reg |= PM_APBCMASK_SERCOM2;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM2_GCLK_ID_CORE) |
                      GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY)
  {
  }

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM2_GCLK_ID_SLOW) |
                      GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY)
  {
  }

  PORT->Group[0].PINCFG[8].bit.PMUXEN = 1U;
  PORT->Group[0].PINCFG[9].bit.PMUXEN = 1U;
  PORT->Group[0].PMUX[4].reg = PORT_PMUX_PMUXE_D | PORT_PMUX_PMUXO_D;

  SERCOM2->I2CM.CTRLA.bit.SWRST = 1U;
  i2c_wait_sync();

  SERCOM2->I2CM.CTRLA.reg =
      SERCOM_I2CM_CTRLA_MODE(SERCOM_I2CM_CTRLA_MODE_I2C_MASTER_Val);
  SERCOM2->I2CM.BAUD.bit.BAUD =
      SystemCoreClock / (2U * I2C_BAUDRATE) - 5U -
      (((SystemCoreClock / 1000000U) * WIRE_RISE_TIME_NANOSECONDS) /
       (2U * 1000U));

  SERCOM2->I2CM.CTRLA.bit.ENABLE = 1U;
  i2c_wait_sync();

  SERCOM2->I2CM.STATUS.bit.BUSSTATE = 1U;
  i2c_wait_sync();

  initialized = true;
}

static uint16_t eccx08_crc16(const uint8_t *data, size_t len)
{
  if (!data || len == 0U)
  {
    return 0U;
  }

  uint16_t crc = 0U;
  while (len > 0U)
  {
    uint8_t b = *data++;
    for (uint8_t shift = 0x01U; shift > 0U; shift <<= 1U)
    {
      uint8_t data_bit = (b & shift) ? 1U : 0U;
      uint8_t crc_bit = (uint8_t)(crc >> 15U);

      crc <<= 1U;
      if (data_bit != crc_bit)
      {
        crc ^= 0x8005U;
      }
    }
    len--;
  }
  return crc;
}

static bool eccx08_send_word_address(uint8_t word_address)
{
  return i2c_write(&word_address, 1U);
}

static bool eccx08_receive_response(void *response, size_t len)
{
  uint8_t response_buffer[len + 3U];
  size_t response_size = sizeof(response_buffer);
  bool response_read = false;

  for (uint8_t retries = 20U; retries > 0U; retries--)
  {
    if (i2c_read(response_buffer, response_size))
    {
      response_read = true;
      break;
    }
  }

  if (!response_read || response_buffer[0] != response_size)
  {
    delay_for(duration_create_milliseconds(1U));
    eccx08_send_word_address(ECCX08_WORD_ADDRESS_IDLE);
    return false;
  }

  uint16_t response_crc = (uint16_t)response_buffer[len + 1U] |
                          ((uint16_t)response_buffer[len + 2U] << 8U);
  if (response_crc != eccx08_crc16(response_buffer, response_size - 2U))
  {
    delay_for(duration_create_milliseconds(1U));
    eccx08_send_word_address(ECCX08_WORD_ADDRESS_IDLE);
    return false;
  }

  memcpy(response, &response_buffer[1], len);
  return true;
}

static bool eccx08_send_command(uint8_t opcode, uint8_t param1, uint16_t param2,
                                const uint8_t *data, size_t len)
{
  uint8_t command[8U + len];
  command[0] = ECCX08_WORD_ADDRESS_COMMAND;
  command[1] = (uint8_t)(sizeof(command) - 1U);
  command[2] = opcode;
  command[3] = param1;
  command[4] = (uint8_t)param2;
  command[5] = (uint8_t)(param2 >> 8U);

  if (data && len > 0U)
  {
    memcpy(&command[6], data, len);
  }

  uint16_t crc = eccx08_crc16(&command[1], 5U + len);
  command[6U + len] = (uint8_t)crc;
  command[7U + len] = (uint8_t)(crc >> 8U);

  return i2c_write(command, sizeof(command));
}

static bool eccx08_wakeup(void)
{
  uint8_t response = 0U;

  i2c_start(0x00U, I2C_WRITE);
  i2c_stop();
  delay_for(duration_create_milliseconds(2U));

  return eccx08_receive_response(&response, sizeof(response)) &&
         response == 0x11U;
}

static void eccx08_idle(void)
{
  eccx08_send_word_address(ECCX08_WORD_ADDRESS_IDLE);
  delay_for(duration_create_milliseconds(1U));
}

static uint32_t eccx08_version(void)
{
  uint32_t version = 0U;

  if (!eccx08_wakeup())
  {
    return 0U;
  }

  if (!eccx08_send_command(ECCX08_OPCODE_INFO, 0x00U, 0x0000U, NULL, 0U))
  {
    return 0U;
  }

  delay_for(duration_create_milliseconds(2U));
  if (!eccx08_receive_response(&version, sizeof(version)))
  {
    return 0U;
  }

  eccx08_idle();
  return version;
}

static bool eccx08_begin(void)
{
  i2c_init();

  if (!eccx08_wakeup())
  {
    return false;
  }
  eccx08_idle();

  uint32_t version = eccx08_version() & ECCX08_VERSION_MASK;
  return version == ECCX08_508_VERSION || version == ECCX08_608_VERSION;
}

static uint16_t eccx08_address_for_slot_offset(uint8_t slot, uint16_t offset)
{
  uint16_t block = offset / ECCX08_BLOCK_SIZE;
  offset = (uint16_t)((offset % ECCX08_BLOCK_SIZE) / ECCX08_WORD_SIZE);
  return (uint16_t)((slot << 3U) | (block << 8U) | offset);
}

static bool eccx08_read_zone(uint8_t zone, uint16_t address, uint8_t *data,
                             size_t len)
{
  if (len != ECCX08_WORD_SIZE && len != ECCX08_BLOCK_SIZE)
  {
    return false;
  }

  if (!eccx08_wakeup())
  {
    return false;
  }

  if (len == ECCX08_BLOCK_SIZE)
  {
    zone |= ECCX08_ZONE_32_BYTES;
  }

  if (!eccx08_send_command(ECCX08_OPCODE_READ, zone, address, NULL, 0U))
  {
    return false;
  }

  delay_for(duration_create_milliseconds(5U));
  if (!eccx08_receive_response(data, len))
  {
    return false;
  }

  eccx08_idle();
  return true;
}

static bool eccx08_write_zone(uint8_t zone, uint16_t address,
                              const uint8_t *data, size_t len)
{
  uint8_t status = 0U;

  if (len != ECCX08_WORD_SIZE && len != ECCX08_BLOCK_SIZE)
  {
    return false;
  }

  if (!eccx08_wakeup())
  {
    return false;
  }

  if (len == ECCX08_BLOCK_SIZE)
  {
    zone |= ECCX08_ZONE_32_BYTES;
  }

  if (!eccx08_send_command(ECCX08_OPCODE_WRITE, zone, address, data, len))
  {
    return false;
  }

  delay_for(duration_create_milliseconds(26U));
  if (!eccx08_receive_response(&status, sizeof(status)))
  {
    return false;
  }

  eccx08_idle();
  return status == 0U;
}

static bool eccx08_read_slot(uint8_t slot, uint8_t *data, size_t len)
{
  if (slot > ECCX08_MAX_SLOT || !data || (len % ECCX08_WORD_SIZE) != 0U)
  {
    return false;
  }

  size_t chunk_size = ECCX08_BLOCK_SIZE;
  for (size_t i = 0U; i < len; i += chunk_size)
  {
    if ((len - i) < ECCX08_BLOCK_SIZE)
    {
      chunk_size = ECCX08_WORD_SIZE;
    }

    if (!eccx08_read_zone(ECCX08_ZONE_DATA,
                          eccx08_address_for_slot_offset(slot, i),
                          &data[i],
                          chunk_size))
    {
      return false;
    }
  }

  return true;
}

static bool eccx08_write_slot(uint8_t slot, const uint8_t *data, size_t len)
{
  if (slot > ECCX08_MAX_SLOT || !data || (len % ECCX08_WORD_SIZE) != 0U)
  {
    return false;
  }

  size_t chunk_size = ECCX08_BLOCK_SIZE;
  for (size_t i = 0U; i < len; i += chunk_size)
  {
    if ((len - i) < ECCX08_BLOCK_SIZE)
    {
      chunk_size = ECCX08_WORD_SIZE;
    }

    if (!eccx08_write_zone(ECCX08_ZONE_DATA,
                           eccx08_address_for_slot_offset(slot, i),
                           &data[i],
                           chunk_size))
    {
      return false;
    }
  }

  return true;
}

static bool eccx08_locked(void)
{
  uint8_t config[4] = {0};
  if (!eccx08_read_zone(ECCX08_ZONE_CONFIG, 0x15U, config, sizeof(config)))
  {
    return false;
  }

  return config[2] == 0x00U && config[3] == 0x00U;
}

static bool eccx08_lock_zone(uint8_t zone)
{
  uint8_t status = 0U;

  if (!eccx08_wakeup())
  {
    return false;
  }

  if (!eccx08_send_command(
          ECCX08_OPCODE_LOCK, (uint8_t)(0x80U | zone), 0x0000U, NULL, 0U))
  {
    return false;
  }

  delay_for(duration_create_milliseconds(32U));
  if (!eccx08_receive_response(&status, sizeof(status)))
  {
    return false;
  }

  eccx08_idle();
  return status == 0U;
}

static bool eccx08_lock(void)
{
  return eccx08_lock_zone(0U) && eccx08_lock_zone(1U);
}

static bool eccx08_check_access(void)
{
  if (!eccx08_begin())
  {
    return false;
  }

  if (!eccx08_locked() && !eccx08_lock())
  {
    return false;
  }

  return true;
}

struct SecretStore *eccx08_create(uint8_t slot)
{
  if (slot > ECCX08_MAX_SLOT)
  {
    return NULL;
  }

  struct SecretStore *result = malloc(sizeof(struct SecretStore));
  if (!result)
  {
    return NULL;
  }
  result->slot = slot;
  return result;
}

size_t secretstore_read_secret(const struct SecretStore *store, uint8_t *secret,
                               size_t len)
{
  if (!store || !secret || len == 0U)
  {
    return 0U;
  }

  if (!eccx08_check_access())
  {
    return 0U;
  }

  if (eccx08_read_slot(store->slot, secret, len))
  {
    delay_for(duration_create_seconds(1U));
    return len;
  }
  return 0U;
}

size_t secretstore_write_secret(const struct SecretStore *store,
                                const uint8_t *secret, size_t len)
{
  if (!store || !secret || len == 0U)
  {
    return 0U;
  }

  if (!eccx08_check_access())
  {
    return 0U;
  }

  if (eccx08_write_slot(store->slot, secret, len))
  {
    delay_for(duration_create_seconds(1U));
    return len;
  }
  return 0U;
}

void secretstore_destroy(struct SecretStore *store)
{
  if (!store)
  {
    return;
  }

  free(store);
}
