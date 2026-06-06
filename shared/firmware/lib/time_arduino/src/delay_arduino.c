/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#include "time/delay.h"
#include <Arduino.h>

void delay_for(duration_t duration)
{
  if (duration.milliseconds > 0)
  {
    delay(duration.milliseconds);
  }
}