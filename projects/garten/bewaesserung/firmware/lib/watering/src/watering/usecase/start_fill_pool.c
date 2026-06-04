/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "start_fill_pool.h"
#include "pump.h"
#include <stddef.h>
#include <stdlib.h>

struct StartFillPool
{
  struct Pump *pump;
};

struct StartFillPool *start_fill_pool_create(struct Pump *pump)
{
  if (!pump)
  {
    return NULL;
  }
  struct StartFillPool *result = malloc(sizeof(struct StartFillPool));
  if (!result)
  {
    return NULL;
  }
  result->pump = pump;
  return result;
}

void start_fill_pool_execute(struct StartFillPool *self)
{
  if (!self)
  {
    return;
  }
  pump_open_pool_valve(self->pump);
}

void start_fill_pool_destroy(struct StartFillPool **self)
{
  if (!self || !*self)
  {
    return;
  }
  free(*self);
  *self = NULL;
}