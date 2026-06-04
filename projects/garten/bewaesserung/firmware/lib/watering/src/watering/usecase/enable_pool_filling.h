/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef ENABLE_POOL_FILLING_H
#define ENABLE_POOL_FILLING_H

#include "pump.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct EnablePoolFilling;

  struct EnablePoolFilling *enable_pool_filling_create(struct Pump *pump);
  void enable_pool_filling_execute(struct EnablePoolFilling *self);
  void enable_pool_filling_destroy(struct EnablePoolFilling **self);

#ifdef __cplusplus
}
#endif
#endif
