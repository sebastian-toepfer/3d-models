/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef TIME_DURATION_H
#define TIME_DURATION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
  uint32_t milliseconds;
} duration_t;

static inline duration_t duration_create_milliseconds(uint32_t value)
{
  return (duration_t){ .milliseconds = value };
}

static inline duration_t duration_create_seconds(uint32_t value)
{
  return duration_create_milliseconds(value * 1000UL);
}

static inline duration_t duration_create_minutes(uint32_t value)
{
  return duration_create_seconds(value * 60UL);
}

static inline duration_t duration_create_hours(uint32_t value)
{
  return duration_create_minutes(value * 60UL);
}

#ifdef __cplusplus
}
#endif
#endif