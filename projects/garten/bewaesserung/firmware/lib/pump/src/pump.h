/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#ifndef PUMP_H
#define PUMP_H
#include "gpio/digital_output_pin.h"
#include "time/duration.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct Pump;

  typedef struct
  {
    const pin_config_t *relay;
    const pin_config_t *lock_relay;
  } pump_valve_config_t;

  typedef struct
  {
    const pin_config_t *main_switch;
    const pump_valve_config_t *garden_valve;
    const pump_valve_config_t *pool_valve;
    const duration_t delay;
  } pump_config_t;

  struct Pump *pump_create(const pump_config_t *config);
  void pump_destroy(struct Pump *pump);

  void pump_open_garden_valve(struct Pump *pump);
  void pump_close_garden_valve(struct Pump *pump);
  void pump_lock_garden_valve(struct Pump *pump);
  void pump_unlock_garden_valve(struct Pump *pump);

  void pump_open_pool_valve(struct Pump *pump);
  void pump_close_pool_valve(struct Pump *pump);
  void pump_lock_pool_valve(struct Pump *pump);
  void pump_unlock_pool_valve(struct Pump *pump);

#ifdef __cplusplus
}
#endif
#endif