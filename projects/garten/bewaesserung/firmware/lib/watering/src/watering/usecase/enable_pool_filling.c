/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "enable_pool_filling.h"
#include "pump.h"
#include <stddef.h>
#include <stdlib.h>

struct EnablePoolFilling
{
  struct Pump *pump;
};

struct EnablePoolFilling *enable_pool_filling_create(struct Pump *pump)
{
  if (!pump)
  {
    return NULL;
  }

  struct EnablePoolFilling *result = malloc(sizeof(struct EnablePoolFilling));
  if (!result)
  {
    return NULL;
  }
  result->pump = pump;
  return result;
}

void enable_pool_filling_execute(struct EnablePoolFilling *self)
{
  if (!self)
  {
    return;
  }
  pump_unlock_pool_valve(self->pump);
}

void enable_pool_filling_destroy(struct EnablePoolFilling **self)
{
  if (!self || !*self)
  {
    return;
  }
  free(*self);
  *self = NULL;
}
