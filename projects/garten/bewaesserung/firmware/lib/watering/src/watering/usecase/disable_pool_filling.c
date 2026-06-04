/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "disable_pool_filling.h"
#include "pump.h"
#include <stddef.h>
#include <stdlib.h>

struct DisablePoolFilling
{
  struct Pump *pump;
};

struct DisablePoolFilling *disable_pool_filling_create(struct Pump *pump)
{
  if (!pump)
  {
    return NULL;
  }
  struct DisablePoolFilling *result = malloc(sizeof(struct DisablePoolFilling));
  if (!result)
  {
    return NULL;
  }
  result->pump = pump;
  return result;
}

void disable_pool_filling_execute(struct DisablePoolFilling *self)
{
  if (!self)
  {
    return;
  }
  pump_lock_pool_valve(self->pump);
}

void disable_pool_filling_destroy(struct DisablePoolFilling **self)
{
  if (!self || !*self)
  {
    return;
  }
  free(*self);
  *self = NULL;
}