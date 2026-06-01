/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef TIME_TIMER_H
#define TIME_TIMER_H

#include "time/duration.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct Timer;

  typedef struct
  {
    const void *platform_info;
  } timer_config_t;

  extern const timer_config_t default_timer_config;

  typedef void (*timer_callback_t)(struct Timer *timer, void *context);

  struct Timer *timer_create(const timer_config_t *config);
  void timer_start(struct Timer *timer, duration_t duration,
                   timer_callback_t callback, void *context);
  void timer_delay_for(struct Timer *timer, duration_t duration);
  void timer_destroy(struct Timer *timer);

#ifdef __cplusplus
}
#endif
#endif
