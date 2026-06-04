/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "connectivity/lora.h"
#include "watering/watering.h"
#include <stddef.h>
#include <stdlib.h>

struct LoraAdapter
{
};

static void handle_garden_valve(uint8_t *data, size_t len, void *context)
{
  struct Watering *watering = context;
  if (len > 0 && data[0] == 0x01)
  {
    watering_start_watering_garden(watering);
  }
  else
  {
    watering_stop_watering_garden(watering);
  }
}

static void handle_pool_valve(uint8_t *data, size_t len, void *context)
{
  struct Watering *watering = context;
  if (len > 0 && data[0] == 0x01)
  {
    watering_start_fill_pool(watering);
  }
  else
  {
    watering_stop_fill_pool(watering);
  }
}

/*
  0x00 - 0000 0000 -> unlocked all (same as unlock pool)
  0x01 - 0000 0001 -> lock garden (not supported yet)
  0x02 - 0000 0010 -> lock pool
  0x03 - 0000 0011 -> lock garden (not supported yet) & lock pool
  ...
  0xFF - 1111 1111 -> lock all
  */
static void handle_lock_valves(uint8_t *data, size_t len, void *context)
{
  if (len <= 0)
  {
    return;
  }

  struct Watering *watering = context;
  if (data[0] & (1 << 1))
  {
    watering_disable_pool_filling(watering);
  }
  else
  {
    watering_enable_pool_filling(watering);
  }
}

struct LoraAdapter *lora_adapter_create(struct LoRa *lora,
                                        struct Watering *watering)
{
  if (!lora || !watering)
  {
    return NULL;
  }

  struct LoraAdapter *result = malloc(sizeof(struct LoraAdapter));
  if (!result)
  {
    return NULL;
  }
  lora_register_handler(lora, 1, handle_garden_valve, watering);
  lora_register_handler(lora, 2, handle_pool_valve, watering);
  lora_register_handler(lora, 3, handle_lock_valves, watering);
  return result;
}

void lora_adapter_destroy(struct LoraAdapter **self)
{
  if (!self || !*self)
  {
    return;
  }
  free(*self);
  *self = NULL;
}
