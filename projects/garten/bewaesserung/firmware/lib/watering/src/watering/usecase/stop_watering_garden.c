/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include <stddef.h>
#include <stdlib.h>

#include "pump.h"
#include "stop_watering_garden.h"

struct StopWateringGarden
{
  struct Pump *pump;
};

struct StopWateringGarden *stop_wattering_garden_create(struct Pump *pump)
{
  if (!pump)
  {
    return NULL;
  }
  struct StopWateringGarden *result = malloc(sizeof(struct StopWateringGarden));
  if (!result)
  {
    return NULL;
  }
  result->pump = pump;
  return result;
}

void stop_wattering_garden_execute(struct StopWateringGarden *self)
{
  if (!self)
  {
    return;
  }
  pump_close_garden_valve(self->pump);
}

void stop_wattering_garden_destroy(struct StopWateringGarden **self)
{
  if (!self || !*self)
  {
    return;
  }
  free(*self);
  *self = NULL;
}