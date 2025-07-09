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

struct Pump *pump_create(
    const digital_pin_config_t *main_switch,
    const digital_pin_config_t *garden_valve_relais,
    const digital_pin_config_t *pool_valve_relais,
    const timeout_config_t *config
);
void pump_destroy(struct Pump *pump);

void pump_open_garden_valve(const struct Pump *pump);
void pump_close_garden_valve(const struct Pump *pump);

void pump_open_pool_valve(const struct Pump *pump);
void pump_close_pool_valve(const struct Pump *pump);

#ifdef __cplusplus
}
#endif
#endif