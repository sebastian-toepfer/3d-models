#include <Arduino.h>
#include <stddef.h>

#include "pump.h"

struct Valve
{
  struct DigitalOutputPin *pin;
  bool open;
};

struct Pump
{
  struct DigitalOutputPin *main_switch;
  struct Valve *garden;
  struct Valve *pool;
  uint16_t on_delay;
  uint16_t off_delay;
};

static inline struct Valve *
pump_create_valve(const digital_pin_config_t *relais)
{
  struct Valve *result = malloc(sizeof(struct Valve));
  if (!result)
  {
    return NULL;
  }
  result->pin = digital_output_pin_create(relais);
  if (!result->pin)
  {
    free(result);
    return NULL;
  }
  result->open = false;
  return result;
}

struct Pump *pump_create(const digital_pin_config_t *main_switch,
                         const digital_pin_config_t *garden_valve_relais,
                         const digital_pin_config_t *pool_valve_relais,
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

  result->garden = pump_create_valve(garden_valve_relais);
  if (!result->garden)
  {
    free(result->main_switch);
    free(result);
  }

  result->pool = pump_create_valve(pool_valve_relais);
  if (!result->pool)
  {
    free(result->garden);
    free(result->main_switch);
    free(result);
  }

  result->on_delay = config->on_delay;
  result->off_delay = config->off_delay;
  return result;
}

// cppcheck-suppress unusedFunction
void pump_destroy(struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  free(pump->pool->pin);
  free(pump->pool);
  free(pump->garden->pin);
  free(pump->garden);
  free(pump->main_switch);
  free(pump);
}

static inline void pump_open_valve(struct Valve *valve)
{
  digital_output_pin_switch_on(valve->pin);
  valve->open = true;
}

static inline void pump_close_value(struct Valve *valve)
{
  digital_output_pin_switch_off(valve->pin);
  valve->open = false;
}

static inline void pump_on(const struct Pump *pump)
{
  delay(pump->on_delay);
  digital_output_pin_switch_on(pump->main_switch);
}

static inline void pump_off(const struct Pump *pump)
{
  digital_output_pin_switch_off(pump->main_switch);
  delay(pump->off_delay);
}

void pump_open_garden_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_open_valve(pump->garden);
  pump_on(pump);
}

void pump_close_garden_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  if (!pump->pool->open)
  {
    pump_off(pump);
  }
  pump_close_value(pump->garden);
}

void pump_open_pool_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  pump_open_valve(pump->pool);
  pump_on(pump);
}

void pump_close_pool_valve(const struct Pump *pump)
{
  if (!pump)
  {
    return;
  }
  if (!pump->garden->open)
  {
    pump_off(pump);
  }
  pump_close_value(pump->pool);
}
