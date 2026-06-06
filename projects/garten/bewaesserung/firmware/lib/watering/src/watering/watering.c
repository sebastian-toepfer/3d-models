/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "watering.h"
#include "pump.h"
#include "usecase/disable_pool_filling.h"
#include "usecase/enable_pool_filling.h"
#include "usecase/start_fill_pool.h"
#include "usecase/start_watering_garden.h"
#include "usecase/stop_fill_pool.h"
#include "usecase/stop_watering_garden.h"
#include <stddef.h>
#include "mem/mem.h"

struct Watering
{
  struct StartWateringGarden *start_watering_garden;
  struct StopWateringGarden *stop_watering_garden;
  struct StartFillPool *start_fill_pool;
  struct StopFillPool *stop_fill_pool;
  struct EnablePoolFilling *enable_pool_filling;
  struct DisablePoolFilling *disable_pool_filling;
};

struct Watering *watering_create(struct Pump *pump)
{
  if (!pump)
  {
    return NULL;
  }
  struct Watering *result = mem_allocate(sizeof(struct Watering));
  if (!result)
  {
    return NULL;
  }
  result->start_watering_garden = start_watering_garden_create(pump);
  result->stop_watering_garden = stop_wattering_garden_create(pump);
  result->start_fill_pool = start_fill_pool_create(pump);
  result->stop_fill_pool = stop_fill_pool_create(pump);
  result->enable_pool_filling = enable_pool_filling_create(pump);
  result->disable_pool_filling = disable_pool_filling_create(pump);
  return result;
}

void watering_start_watering_garden(struct Watering *self)
{
  if (!self)
  {
    return;
  }
  start_watering_garden_execute(self->start_watering_garden);
}

void watering_stop_watering_garden(struct Watering *self)
{
  if (!self)
  {
    return;
  }
  stop_wattering_garden_execute(self->stop_watering_garden);
}

void watering_start_fill_pool(struct Watering *self)
{
  if (!self)
  {
    return;
  }
  start_fill_pool_execute(self->start_fill_pool);
}

void watering_stop_fill_pool(struct Watering *self)
{
  if (!self)
  {
    return;
  }
  stop_fill_pool_execute(self->stop_fill_pool);
}

void watering_enable_pool_filling(struct Watering *self)
{
  if (!self)
  {
    return;
  }
  enable_pool_filling_execute(self->enable_pool_filling);
}

void watering_disable_pool_filling(struct Watering *self)
{
  if (!self)
  {
    return;
  }
  disable_pool_filling_execute(self->disable_pool_filling);
}

void watering_destroy(struct Watering **self)
{
  if (!self || !*self)
  {
    return;
  }
  start_watering_garden_destroy(&(*self)->start_watering_garden);
  stop_wattering_garden_destroy(&(*self)->stop_watering_garden);
  start_fill_pool_destroy(&(*self)->start_fill_pool);
  stop_fill_pool_destroy(&(*self)->stop_fill_pool);
  enable_pool_filling_destroy(&(*self)->enable_pool_filling);
  disable_pool_filling_destroy(&(*self)->disable_pool_filling);
  mem_free(self);
}
