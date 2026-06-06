/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#include <stddef.h>

#include "pump.h"
#include "time/delay.h"
#include "time/duration.h"
#include "valve.h"
#include "mem/mem.h"

struct Pump
{
  struct DigitalOutputPin *main_switch;
  struct LockableValve *garden;
  struct LockableValve *pool;
  duration_t delay;
};

struct Pump *pump_create(const pump_config_t *config)
{
  if (!config || !config->main_switch || !config->garden_valve ||
      !config->pool_valve)
  {
    return NULL;
  }

  struct Pump *result = mem_allocate(sizeof(struct Pump));
  if (!result)
  {
    return NULL;
  }
  result->main_switch = digital_output_pin_create(config->main_switch);
  if (!result->main_switch)
  {
    mem_free(result);
    return NULL;
  }

  result->garden = lockable_valve_create(&(lockable_valve_config_t){
      .valve = &(valve_config_t){.pin = config->garden_valve->relay,
                                 .delay = config->delay},
      .pin = config->garden_valve->lock_relay});
  if (!result->garden)
  {
    digital_output_pin_destroy(&result->main_switch);
    mem_free(result);
    return NULL;
  }

  result->pool = lockable_valve_create(&(lockable_valve_config_t){
      .valve = &(valve_config_t){.pin = config->pool_valve->relay,
                                 .delay = config->delay},
      .pin = config->pool_valve->lock_relay});
  if (!result->pool)
  {
    lockable_valve_destroy(&result->garden);
    digital_output_pin_destroy(&result->main_switch);
    mem_free(result);
    return NULL;
  }

  result->delay = config->delay;
  return result;
}

// cppcheck-suppress unusedFunction
void pump_destroy(struct Pump **pump)
{
  if (!pump || !*pump)
  {
    return;
  }
  lockable_valve_destroy(&(*pump)->pool);
  lockable_valve_destroy(&(*pump)->garden);
  digital_output_pin_destroy(&(*pump)->main_switch);
  mem_free(*pump);
  *pump = NULL;
}

static inline void pump_valve_close(const struct Pump *pump,
                                    struct LockableValve *valve)
{
  // we use it as an opener, so we must open it for a short period
  digital_output_pin_switch_on(pump->main_switch);
  delay_for(pump->delay);
  lockable_valve_close(valve);
  digital_output_pin_switch_off(pump->main_switch);
}

void pump_open_garden_valve(struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  lockable_valve_open(pump->garden);
}

void pump_close_garden_valve(struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  if (lockable_valve_is_open(pump->garden))
  {
    pump_valve_close(pump, pump->garden);
    if (lockable_valve_is_open(pump->pool))
    {
      lockable_valve_open(pump->pool);
    }
  }
}

void pump_lock_garden_valve(struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_close_garden_valve(pump);
  lockable_valve_lock(pump->garden);
}

void pump_unlock_garden_valve(struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  lockable_valve_unlock(pump->garden);
}

void pump_open_pool_valve(struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  lockable_valve_open(pump->pool);
}

void pump_close_pool_valve(struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  if (lockable_valve_is_open(pump->pool))
  {
    pump_valve_close(pump, pump->pool);
    if (lockable_valve_is_open(pump->garden))
    {
      lockable_valve_open(pump->garden);
    }
  }
}

void pump_lock_pool_valve(struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_close_pool_valve(pump);
  lockable_valve_lock(pump->pool);
}

void pump_unlock_pool_valve(struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  lockable_valve_unlock(pump->pool);
}