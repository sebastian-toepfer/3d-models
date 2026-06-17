/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#ifndef PUMP_H
#define PUMP_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "digital_output_pin.h"

struct Pump;

typedef struct
{
    uint16_t on_delay;
    uint16_t off_delay;
} timeout_config_t;

typedef struct
{
    const digital_pin_config_t *relay;
    const digital_pin_config_t *lock_relay;
} lockable_valve_t;

struct Pump *pump_create(
    const digital_pin_config_t *main_switch,
    const lockable_valve_t *garden_valve,
    const lockable_valve_t *pool_valve,
    const timeout_config_t *config
);
void pump_destroy(struct Pump *pump);

void pump_open_garden_valve(const struct Pump *pump);
void pump_close_garden_valve(const struct Pump *pump);
void pump_lock_garden_valve(const struct Pump *pump);
void pump_unlock_garden_valve(const struct Pump *pump);

void pump_open_pool_valve(const struct Pump *pump);
void pump_close_pool_valve(const struct Pump *pump);
void pump_lock_pool_valve(const struct Pump *pump);
void pump_unlock_pool_valve(const struct Pump *pump);

#ifdef __cplusplus
}
#endif
#endif