/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef LOCK_POOL_VALVE_H
#define LOCK_POOL_VALVE_H

#include "pump.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct DisablePoolFilling;

  struct DisablePoolFilling *disable_pool_filling_create(struct Pump *pump);
  void disable_pool_filling_execute(struct DisablePoolFilling *self);
  void disable_pool_filling_destroy(struct DisablePoolFilling **self);

#ifdef __cplusplus
}
#endif
#endif
