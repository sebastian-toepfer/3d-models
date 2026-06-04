/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef START_FILL_POOL_H
#define START_FILL_POOL_H

#include "pump.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct StartFillPool;

  struct StartFillPool *start_fill_pool_create(struct Pump *pump);
  void start_fill_pool_execute(struct StartFillPool *self);
  void start_fill_pool_destroy(struct StartFillPool **self);

#ifdef __cplusplus
}
#endif
#endif
