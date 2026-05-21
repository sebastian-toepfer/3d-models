/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#include <stddef.h>
#include <stdlib.h>

#include "platform/sleep.h"
#include "pump.h"

struct Valve
{
  struct DigitalOutputPin *pin;
  struct DigitalOutputPin *lock;
  uint16_t delay;
  bool open;
};

struct Pump
{
  struct DigitalOutputPin *main_switch;
  struct Valve *garden;
  struct Valve *pool;
  uint16_t delay;
};

static inline struct Valve *
pump_valve_create(const lockable_valve_t *valve_pins, const uint16_t delay)
{
  struct Valve *result = malloc(sizeof(struct Valve));
  if (!result)
  {
    return NULL;
  }
  result->pin = digital_output_pin_create(valve_pins->relay);
  if (!result->pin)
  {
    free(result);
    return NULL;
  }
  if (valve_pins->lock_relay)
  {
    result->lock = digital_output_pin_create(valve_pins->lock_relay);
    if (!result->lock)
    {
      digital_output_pin_destroy(result->pin);
      free(result);
      return NULL;
    }
    digital_output_pin_switch_on(result->lock);
  }
  else
  {
    result->lock = NULL;
  }
  result->delay = delay;
  result->open = false;
  return result;
}

static inline void pump_valve_destroy(struct Valve *valve)
{
  if (valve->pin)
  {
    digital_output_pin_destroy(valve->pin);
  }

  if (valve->lock)
  {
    digital_output_pin_destroy(valve->lock);
  }
  free(valve);
}

static inline void pump_valve_open(struct Valve *valve)
{
  if (!valve) {
    return;
  }
  digital_output_pin_switch_on(valve->pin);
  sleep_ms(valve->delay);
  valve->open = true;
  digital_output_pin_switch_off(valve->pin);
}

// close need access to pump

static inline void pump_valve_lock(struct Valve *valve)
{
  if (valve->lock)
  {
    // is inverted, in the hope that it is so better for the whole system
    digital_output_pin_switch_off(valve->lock);
  }
}

static inline void pump_valve_unlock(const struct Valve *valve)
{
  if (valve->lock)
  {
    digital_output_pin_switch_on(valve->lock);
  }
}

struct Pump *pump_create(const digital_pin_config_t *main_switch,
                         const lockable_valve_t *garden_valve_relais,
                         const lockable_valve_t *pool_valve_relais,
                         const timeout_config_t *config)
{
  if (!main_switch || !garden_valve_relais || !pool_valve_relais || !config)
  {
    return NULL;
  }

  struct Pump *result = malloc(sizeof(struct Pump));
  if (!result)
  {
    return NULL;
  }
  result->main_switch = digital_output_pin_create(main_switch);
  if (!result->main_switch)
  {
    free(result);
    return NULL;
  }

  result->garden = pump_valve_create(garden_valve_relais, config->on_delay);
  if (!result->garden)
  {
    digital_output_pin_destroy(result->main_switch);
    free(result);
    return NULL;
  }

  result->pool = pump_valve_create(pool_valve_relais, config->on_delay);
  if (!result->pool)
  {
    pump_valve_destroy(result->garden);
    digital_output_pin_destroy(result->main_switch);
    free(result);
    return NULL;
  }

  result->delay = config->on_delay;
  return result;
}

// cppcheck-suppress unusedFunction
void pump_destroy(struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_valve_destroy(pump->pool);
  pump_valve_destroy(pump->garden);
  digital_output_pin_destroy(pump->main_switch);
  free(pump);
}

static inline void pump_valve_close(const struct Pump *pump,
                                    struct Valve *valve)
{
  // we use it as an opener, so we must open it for a short period
  digital_output_pin_switch_on(pump->main_switch);
  sleep_ms(pump->delay);
  valve->open = false;
  digital_output_pin_switch_off(pump->main_switch);
}

void pump_open_garden_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_valve_open(pump->garden);
}

void pump_close_garden_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_valve_close(pump, pump->garden);
  if (pump->pool->open)
  {
    pump_valve_open(pump->pool);
  }
}

void pump_lock_garden_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_close_garden_valve(pump);
  pump_valve_lock(pump->garden);
}

void pump_unlock_garden_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_valve_unlock(pump->garden);
}

void pump_open_pool_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_valve_open(pump->pool);
}

void pump_close_pool_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_valve_close(pump, pump->pool);
  if (pump->garden->open)
  {
    pump_valve_open(pump->garden);
  }
}

void pump_lock_pool_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_close_pool_valve(pump);
  pump_valve_lock(pump->pool);
}

void pump_unlock_pool_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_valve_unlock(pump->pool);
}