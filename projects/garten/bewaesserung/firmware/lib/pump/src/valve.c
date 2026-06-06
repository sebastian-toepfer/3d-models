/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include <stddef.h>
#include <stdlib.h>

#include "gpio/digital_output_pin.h"
#include "time/delay.h"
#include "time/duration.h"
#include "valve.h"

struct Valve
{
  struct DigitalOutputPin *pin;
  duration_t delay;
  bool open;
};

struct LockableValve
{
  struct Valve *valve;
  struct DigitalOutputPin *lock;
  bool locked;
};

struct Valve *valve_create(const valve_config_t *config)
{
  if (!config || !config->pin)
  {
    return NULL;
  }

  struct Valve *result = malloc(sizeof(struct Valve));
  if (!result)
  {
    return NULL;
  }
  result->pin = digital_output_pin_create(config->pin);
  if (!result->pin)
  {
    free(result);
    return NULL;
  }
  result->delay = config->delay;
  result->open = false;
  return result;
}

void valve_open(struct Valve *self)
{
  if (!self)
  {
    return;
  }
  digital_output_pin_switch_on(self->pin);
  delay_for(self->delay);
  self->open = true;
  digital_output_pin_switch_off(self->pin);
}

void valve_close(struct Valve *self)
{
  if (!self)
  {
    return;
  }
  self->open = false;
};

bool valve_is_open(const struct Valve *self)
{
  return self && self->open;
}

bool valve_is_close(const struct Valve *self)
{
  return !self || !self->open;
}

void valve_destroy(struct Valve *self)
{
  if (!self)
  {
    return;
  }
  digital_output_pin_destroy(self->pin);
  free(self);
}

struct LockableValve *
lockable_valve_create(const lockable_valve_config_t *config)
{
  if (!config || !config->valve)
  {
    return NULL;
  }
  struct Valve *valve = valve_create(config->valve);
  if (!valve)
  {
    return NULL;
  }

  struct DigitalOutputPin *lock;
  if (config->pin)
  {
    lock = digital_output_pin_create(config->pin);
    if (!lock)
    {
      valve_destroy(valve);
      return NULL;
    }
  }
  else
  {
    lock = NULL;
  }

  struct LockableValve *result = malloc(sizeof(struct LockableValve));
  if (!result)
  {
    valve_destroy(valve);
    if (lock)
    {
      digital_output_pin_destroy(lock);
    }
    return NULL;
  }
  digital_output_pin_switch_on(lock);
  result->valve = valve;
  result->lock = lock;
  result->locked = false;
  return result;
}

void lockable_valve_open(struct LockableValve *self)
{
  if (!self)
  {
    return;
  }
  valve_open(self->valve);
}

void lockable_valve_close(struct LockableValve *self)
{
  if (!self)
  {
    return;
  }
  valve_close(self->valve);
}

void lockable_valve_lock(struct LockableValve *self)
{
  if (!self || !self->lock)
  {
    return;
  }
  digital_output_pin_switch_off(self->lock);
  self->locked = true;
}

void lockable_valve_unlock(struct LockableValve *self)
{
  if (!self || !self->lock)
  {
    return;
  }
  digital_output_pin_switch_on(self->lock);
  self->locked = false;
}

bool lockable_valve_is_open(const struct LockableValve *self)
{
  return self && valve_is_open(self->valve);
}

bool lockable_valve_is_close(const struct LockableValve *self)
{
  return self && valve_is_close(self->valve);
}

bool lockable_valve_is_locked(const struct LockableValve *self)
{
  return self && self->locked;
}

bool lockable_valve_is_unlocked(const struct LockableValve *self)
{
  return !self || !self->locked;
}

void lockable_valve_destroy(struct LockableValve *self)
{
  if (!self)
  {
    return;
  }
  valve_destroy(self->valve);
  digital_output_pin_destroy(self->lock);
  free(self);
}