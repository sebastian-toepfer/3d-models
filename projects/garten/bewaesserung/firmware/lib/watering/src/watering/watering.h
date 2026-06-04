/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef WATERING_H
#define WATERING_H

#include "pump.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct Watering;

  struct Watering *watering_create(struct Pump *pump);
  void watering_start_watering_garden(struct Watering *self);
  void watering_stop_watering_garden(struct Watering *self);
  void watering_start_fill_pool(struct Watering *self);
  void watering_stop_fill_pool(struct Watering *self);
  void watering_enable_pool_filling(struct Watering *self);
  void watering_disable_pool_filling(struct Watering *self);
  void watering_destroy(struct Watering **self);

#ifdef __cplusplus
}
#endif
#endif