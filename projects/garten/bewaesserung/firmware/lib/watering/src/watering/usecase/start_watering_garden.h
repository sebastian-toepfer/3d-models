/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef START_WATERING_GARDEN_H
#define START_WATERING_GARDEN_H

#include "pump.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct StartWateringGarden;

  struct StartWateringGarden *start_watering_garden_create(struct Pump *pump);
  void start_watering_garden_execute(struct StartWateringGarden *self);
  void start_watering_garden_destroy(struct StartWateringGarden **self);

#ifdef __cplusplus
}
#endif
#endif
