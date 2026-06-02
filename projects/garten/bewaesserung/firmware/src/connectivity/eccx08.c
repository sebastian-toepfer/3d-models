/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "eccx08.h"

#include "i2c_peripheral.h"
#include "time/timer.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

struct ECCX08
{
  struct I2CPeripheral *i2c;
  struct Timer *timer;
};

static bool eccx08_i2c_write(struct ECCX08 *eccx08, const uint8_t *data,
                             size_t len)
{
  return i2c_peripheral_write(eccx08->i2c, data, len);
}

static bool eccx08_i2c_read(struct ECCX08 *eccx08, uint8_t *data, size_t len)
{
  return i2c_peripheral_read(eccx08->i2c, data, len);
}

static bool eccx08_i2c_start_stop(struct ECCX08 *eccx08)
{
  return i2c_peripheral_start_stop_at(eccx08->i2c, 0x00U);
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

static bool eccx08_send_word_address(struct ECCX08 *eccx08,
                                     uint8_t word_address)
{
  return eccx08_i2c_write(eccx08, &word_address, 1U);
}

static bool eccx08_receive_response(struct ECCX08 *eccx08, void *response,
                                    size_t len)
{
  uint8_t response_buffer[len + 3U];
  size_t response_size = sizeof(response_buffer);
  bool response_read = false;

  for (uint8_t retries = 20U; retries > 0U; retries--)
  {
    if (eccx08_i2c_read(eccx08, response_buffer, response_size))
    {
      response_read = true;
      break;
    }
  }

  if (!response_read || response_buffer[0] != response_size)
  {
    timer_delay_for(eccx08->timer, duration_create_milliseconds(1U));
    eccx08_send_word_address(eccx08, ECCX08_WORD_ADDRESS_IDLE);
    return false;
  }

  uint16_t response_crc = (uint16_t)response_buffer[len + 1U] |
                          ((uint16_t)response_buffer[len + 2U] << 8U);
  if (response_crc != eccx08_crc16(response_buffer, response_size - 2U))
  {
    timer_delay_for(eccx08->timer, duration_create_milliseconds(1U));
    eccx08_send_word_address(eccx08, ECCX08_WORD_ADDRESS_IDLE);
    return false;
  }

  memcpy(response, &response_buffer[1], len);
  return true;
}

static bool eccx08_send_command(struct ECCX08 *eccx08, uint8_t opcode,
                                uint8_t param1, uint16_t param2,
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

  return eccx08_i2c_write(eccx08, command, sizeof(command));
}

static bool eccx08_wakeup(struct ECCX08 *eccx08)
{
  uint8_t response = 0U;

  eccx08_i2c_start_stop(eccx08);
  timer_delay_for(eccx08->timer, duration_create_milliseconds(2U));

  return eccx08_receive_response(eccx08, &response, sizeof(response)) &&
         response == 0x11U;
}

static void eccx08_idle(struct ECCX08 *eccx08)
{
  eccx08_send_word_address(eccx08, ECCX08_WORD_ADDRESS_IDLE);
  timer_delay_for(eccx08->timer, duration_create_milliseconds(1U));
}

static uint32_t eccx08_version(struct ECCX08 *eccx08)
{
  uint32_t version = 0U;

  if (!eccx08_wakeup(eccx08))
  {
    return 0U;
  }

  if (!eccx08_send_command(
          eccx08, ECCX08_OPCODE_INFO, 0x00U, 0x0000U, NULL, 0U))
  {
    return 0U;
  }

  timer_delay_for(eccx08->timer, duration_create_milliseconds(2U));
  if (!eccx08_receive_response(eccx08, &version, sizeof(version)))
  {
    return 0U;
  }

  eccx08_idle(eccx08);
  return version;
}

bool eccx08_begin(struct ECCX08 *eccx08)
{
  if (!eccx08)
  {
    return false;
  }

  if (!eccx08_wakeup(eccx08))
  {
    return false;
  }
  eccx08_idle(eccx08);

  uint32_t version = eccx08_version(eccx08) & ECCX08_VERSION_MASK;
  return version == ECCX08_508_VERSION || version == ECCX08_608_VERSION;
}

static uint16_t eccx08_address_for_slot_offset(uint8_t slot, uint16_t offset)
{
  uint16_t block = offset / ECCX08_BLOCK_SIZE;
  offset = (uint16_t)((offset % ECCX08_BLOCK_SIZE) / ECCX08_WORD_SIZE);
  return (uint16_t)((slot << 3U) | (block << 8U) | offset);
}

static bool eccx08_read_zone(struct ECCX08 *eccx08, uint8_t zone,
                             uint16_t address, uint8_t *data, size_t len)
{
  if (len != ECCX08_WORD_SIZE && len != ECCX08_BLOCK_SIZE)
  {
    return false;
  }

  if (!eccx08_wakeup(eccx08))
  {
    return false;
  }

  if (len == ECCX08_BLOCK_SIZE)
  {
    zone |= ECCX08_ZONE_32_BYTES;
  }

  if (!eccx08_send_command(eccx08, ECCX08_OPCODE_READ, zone, address, NULL, 0U))
  {
    return false;
  }

  timer_delay_for(eccx08->timer, duration_create_milliseconds(5U));
  if (!eccx08_receive_response(eccx08, data, len))
  {
    return false;
  }

  eccx08_idle(eccx08);
  return true;
}

static bool eccx08_write_zone(struct ECCX08 *eccx08, uint8_t zone,
                              uint16_t address, const uint8_t *data, size_t len)
{
  uint8_t status = 0U;

  if (len != ECCX08_WORD_SIZE && len != ECCX08_BLOCK_SIZE)
  {
    return false;
  }

  if (!eccx08_wakeup(eccx08))
  {
    return false;
  }

  if (len == ECCX08_BLOCK_SIZE)
  {
    zone |= ECCX08_ZONE_32_BYTES;
  }

  if (!eccx08_send_command(
          eccx08, ECCX08_OPCODE_WRITE, zone, address, data, len))
  {
    return false;
  }

  timer_delay_for(eccx08->timer, duration_create_milliseconds(26U));
  if (!eccx08_receive_response(eccx08, &status, sizeof(status)))
  {
    return false;
  }

  eccx08_idle(eccx08);
  return status == 0U;
}

bool eccx08_read_slot(struct ECCX08 *eccx08, uint8_t slot, uint8_t *data,
                      size_t len)
{
  if (!eccx08 || slot > ECCX08_MAX_SLOT || !data ||
      (len % ECCX08_WORD_SIZE) != 0U)
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

    if (!eccx08_read_zone(eccx08,
                          ECCX08_ZONE_DATA,
                          eccx08_address_for_slot_offset(slot, i),
                          &data[i],
                          chunk_size))
    {
      return false;
    }
  }

  return true;
}

bool eccx08_write_slot(struct ECCX08 *eccx08, uint8_t slot, const uint8_t *data,
                       size_t len)
{
  if (!eccx08 || slot > ECCX08_MAX_SLOT || !data ||
      (len % ECCX08_WORD_SIZE) != 0U)
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

    if (!eccx08_write_zone(eccx08,
                           ECCX08_ZONE_DATA,
                           eccx08_address_for_slot_offset(slot, i),
                           &data[i],
                           chunk_size))
    {
      return false;
    }
  }

  return true;
}

static bool eccx08_locked(struct ECCX08 *eccx08)
{
  uint8_t config[4] = {0};
  if (!eccx08_read_zone(
          eccx08, ECCX08_ZONE_CONFIG, 0x15U, config, sizeof(config)))
  {
    return false;
  }

  return config[2] == 0x00U && config[3] == 0x00U;
}

static bool eccx08_lock_zone(struct ECCX08 *eccx08, uint8_t zone)
{
  uint8_t status = 0U;

  if (!eccx08_wakeup(eccx08))
  {
    return false;
  }

  if (!eccx08_send_command(eccx08,
                           ECCX08_OPCODE_LOCK,
                           (uint8_t)(0x80U | zone),
                           0x0000U,
                           NULL,
                           0U))
  {
    return false;
  }

  timer_delay_for(eccx08->timer, duration_create_milliseconds(32U));
  if (!eccx08_receive_response(eccx08, &status, sizeof(status)))
  {
    return false;
  }

  eccx08_idle(eccx08);
  return status == 0U;
}

static bool eccx08_lock(struct ECCX08 *eccx08)
{
  return eccx08_lock_zone(eccx08, 0U) && eccx08_lock_zone(eccx08, 1U);
}

bool eccx08_check_access(struct ECCX08 *eccx08)
{
  if (!eccx08_begin(eccx08))
  {
    return false;
  }

  if (!eccx08_locked(eccx08) && !eccx08_lock(eccx08))
  {
    return false;
  }

  return true;
}

struct ECCX08 *eccx08_create(const eccx08_config_t *config)
{
  if (!config || !config->i2c || !config->timer)
  {
    return NULL;
  }

  struct ECCX08 *result = malloc(sizeof(struct ECCX08));
  if (!result)
  {
    return NULL;
  }
  result->i2c = config->i2c;
  result->timer = config->timer;
  return result;
}

void eccx08_destroy(struct ECCX08 *eccx08)
{
  if (!eccx08)
  {
    return;
  }
  free(eccx08);
}
