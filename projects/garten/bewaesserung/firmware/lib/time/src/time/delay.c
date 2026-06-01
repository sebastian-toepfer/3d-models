/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "time/delay.h"
#include "time/timer.h"

void delay_for(duration_t duration)
{
  static struct Timer *timer_instance;

  if (!timer_instance)
  {
    timer_instance = timer_create(&default_timer_config);
  }

  timer_delay_for(timer_instance, duration);
}
