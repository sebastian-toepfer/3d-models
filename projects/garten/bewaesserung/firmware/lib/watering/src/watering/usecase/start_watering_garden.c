/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "start_watering_garden.h"
#include "pump.h"
#include <stddef.h>
#include <stdlib.h>

struct StartWateringGarden
{
  struct Pump *pump;
};

struct StartWateringGarden *start_watering_garden_create(struct Pump *pump)
{
  if (!pump)
  {
    return NULL;
  }
  struct StartWateringGarden *result =
      malloc(sizeof(struct StartWateringGarden));
  if (!result)
  {
    return NULL;
  }
  result->pump = pump;
  return result;
}

void start_watering_garden_execute(struct StartWateringGarden *self)
{
  if (!self)
  {
    return;
  }
  pump_open_garden_valve(self->pump);
}

void start_watering_garden_destroy(struct StartWateringGarden **self)
{
  if (!self || !*self)
  {
    return;
  }
  free(*self);
  *self = NULL;
}