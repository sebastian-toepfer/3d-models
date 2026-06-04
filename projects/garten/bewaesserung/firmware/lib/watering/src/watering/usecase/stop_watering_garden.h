/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef STOP_WATERING_GARDEN_H
#define STOP_WATERING_GARDEN_H

#include "pump.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct StopWateringGarden;

  struct StopWateringGarden *stop_wattering_garden_create(struct Pump *pump);
  void stop_wattering_garden_execute(struct StopWateringGarden *self);
  void stop_wattering_garden_destroy(struct StopWateringGarden **self);

#ifdef __cplusplus
}
#endif
#endif
