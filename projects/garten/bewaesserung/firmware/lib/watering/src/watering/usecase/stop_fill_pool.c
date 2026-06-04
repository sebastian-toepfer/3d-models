/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "stop_fill_pool.h"
#include "pump.h"
#include <stddef.h>
#include <stdlib.h>

struct StopFillPool
{
  struct Pump *pump;
};

struct StopFillPool *stop_fill_pool_create(struct Pump *pump)
{
  if (!pump)
  {
    return NULL;
  }
  struct StopFillPool *result = malloc(sizeof(struct StopFillPool));
  if (!result)
  {
    return NULL;
  }
  result->pump = pump;
  return result;
}

void stop_fill_pool_execute(struct StopFillPool *self)
{
  if (!self)
  {
    return;
  }
  pump_close_pool_valve(self->pump);
}

void stop_fill_pool_destroy(struct StopFillPool **self)
{
  if (!self || !*self)
  {
    return;
  }
  free(*self);
  *self = NULL;
}