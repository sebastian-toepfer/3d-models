/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef VALVE_H
#define VALVE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "digital_output_pin.h"
#include "time/duration.h"

  struct Valve;
  struct LockableValve;

  typedef struct
  {
    const digital_pin_config_t *pin;
    const duration_t delay;
  } valve_config_t;

  typedef struct
  {
    const valve_config_t *valve;
    const digital_pin_config_t *pin;
  } lockable_valve_config_t;

  struct Valve *valve_create(const valve_config_t *config);
  void valve_open(struct Valve *self);
  void valve_close(struct Valve *self);
  bool valve_is_open(const struct Valve *self);
  bool valve_is_close(const struct Valve *self);
  void valve_destroy(struct Valve *self);

  struct LockableValve *
  lockable_valve_create(const lockable_valve_config_t *config);
  void lockable_valve_open(struct LockableValve *self);
  void lockable_valve_close(struct LockableValve *self);
  void lockable_valve_lock(struct LockableValve *self);
  void lockable_valve_unlock(struct LockableValve *self);
  bool lockable_valve_is_open(const struct LockableValve *self);
  bool lockable_valve_is_close(const struct LockableValve *self);
  bool lockable_valve_is_locked(const struct LockableValve *self);
  bool lockable_valve_is_unlocked(const struct LockableValve *self);
  void lockable_valve_destroy(struct LockableValve *self);

#ifdef __cplusplus
}
#endif
#endif