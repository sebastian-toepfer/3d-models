/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef STOP_FILLING_POOL_H
#define STOP_FILLING_POOL_H

#include "pump.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct StopFillPool;

  struct StopFillPool *stop_fill_pool_create(struct Pump *pump);
  void stop_fill_pool_execute(struct StopFillPool *self);
  void stop_fill_pool_destroy(struct StopFillPool **self);

#ifdef __cplusplus
}
#endif
#endif
