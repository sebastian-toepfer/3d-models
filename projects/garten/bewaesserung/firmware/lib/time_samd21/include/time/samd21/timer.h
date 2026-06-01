/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef TIMER_SAMD21_H
#define TIMER_SAMD21_H

#include "sam.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef enum
  {
    TIMER_COUNTER_MODE_16BIT,
    TIMER_COUNTER_MODE_32BIT
  } TimerCounterMode_SAMD21;

  typedef struct
  {
    Tc *timer_counter;
    TimerCounterMode_SAMD21 counter_mode;
    uint32_t peripheral_mask;
    uint16_t generic_clock_id;
    uint16_t prescaler;
    uint32_t ticks_per_millisecond;
    IRQn_Type interrupt_number;
  } TimerInfo_SAMD21;

#ifdef __cplusplus
}
#endif
#endif
