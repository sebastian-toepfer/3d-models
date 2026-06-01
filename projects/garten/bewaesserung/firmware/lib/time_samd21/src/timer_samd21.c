#include "sam.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "time/duration.h"
#include "time/samd21/timer.h"
#include "time/timer.h"

struct Timer
{
  Tc *timer_counter;
  TimerCounterMode_SAMD21 counter_mode;
  uint32_t peripheral_mask;
  uint16_t generic_clock_id;
  uint16_t prescaler;
  uint32_t ticks_per_millisecond;
  IRQn_Type interrupt_number;
  volatile uint32_t elapsed_milliseconds;
  uint32_t remaining_milliseconds;
  uint32_t active_chunk_milliseconds;
  uint32_t max_chunk_milliseconds;
  timer_callback_t callback;
  void *callback_context;
  bool callback_active;
};

#define TIMER_REGISTRY_SIZE 3U

static struct Timer *timer_registry[TIMER_REGISTRY_SIZE];

static void wait_for_sync(const struct Timer *timer)
{
  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    while (timer->timer_counter->COUNT32.STATUS.bit.SYNCBUSY)
    {
    }
  }
  else
  {
    while (timer->timer_counter->COUNT16.STATUS.bit.SYNCBUSY)
    {
    }
  }
}

static void timer_reset_counter(const struct Timer *timer)
{
  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    timer->timer_counter->COUNT32.CTRLA.reg = TC_CTRLA_SWRST;
  }
  else
  {
    timer->timer_counter->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  }
}

static void timer_configure_counter(const struct Timer *timer)
{
  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    timer->timer_counter->COUNT32.CTRLA.reg =
        TC_CTRLA_MODE_COUNT32 | TC_CTRLA_WAVEGEN_MFRQ | timer->prescaler;
  }
  else
  {
    timer->timer_counter->COUNT16.CTRLA.reg =
        TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MFRQ | timer->prescaler;
  }
}

static void timer_set_compare(const struct Timer *timer, uint32_t ticks)
{
  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    timer->timer_counter->COUNT32.CC[0].reg = ticks;
  }
  else
  {
    timer->timer_counter->COUNT16.CC[0].reg = (uint16_t)ticks;
  }
}

static void timer_reset_count(const struct Timer *timer)
{
  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    timer->timer_counter->COUNT32.COUNT.reg = 0U;
  }
  else
  {
    timer->timer_counter->COUNT16.COUNT.reg = 0U;
  }
}

static void timer_clear_interrupt_flag(const struct Timer *timer)
{
  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    timer->timer_counter->COUNT32.INTFLAG.reg = TC_INTFLAG_MC0;
  }
  else
  {
    timer->timer_counter->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;
  }
}

static bool timer_has_compare_interrupt(const struct Timer *timer)
{
  bool has_interrupt;

  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    has_interrupt =
        (timer->timer_counter->COUNT32.INTFLAG.reg & TC_INTFLAG_MC0) != 0U;
  }
  else
  {
    has_interrupt =
        (timer->timer_counter->COUNT16.INTFLAG.reg & TC_INTFLAG_MC0) != 0U;
  }

  return has_interrupt;
}

static void timer_enable_compare_interrupt(const struct Timer *timer)
{
  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    timer->timer_counter->COUNT32.INTENSET.reg = TC_INTENSET_MC0;
  }
  else
  {
    timer->timer_counter->COUNT16.INTENSET.reg = TC_INTENSET_MC0;
  }
}

static void timer_disable_compare_interrupt(const struct Timer *timer)
{
  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    timer->timer_counter->COUNT32.INTENCLR.reg = TC_INTENCLR_MC0;
  }
  else
  {
    timer->timer_counter->COUNT16.INTENCLR.reg = TC_INTENCLR_MC0;
  }
}

static void timer_enable_counter(const struct Timer *timer)
{
  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    timer->timer_counter->COUNT32.CTRLA.reg |= TC_CTRLA_ENABLE;
  }
  else
  {
    timer->timer_counter->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  }
}

static uint32_t timer_max_chunk_milliseconds(const struct Timer *timer)
{
  uint32_t max_chunk_milliseconds;

  if (timer->counter_mode == TIMER_COUNTER_MODE_32BIT)
  {
    max_chunk_milliseconds = UINT32_MAX / timer->ticks_per_millisecond;
  }
  else
  {
    max_chunk_milliseconds =
        ((uint32_t)UINT16_MAX + 1U) / timer->ticks_per_millisecond;
  }

  return max_chunk_milliseconds;
}

static void timer_schedule_next_chunk(struct Timer *timer)
{
  if (timer->remaining_milliseconds > timer->max_chunk_milliseconds)
  {
    timer->active_chunk_milliseconds = timer->max_chunk_milliseconds;
  }
  else
  {
    timer->active_chunk_milliseconds = timer->remaining_milliseconds;
  }

  timer_reset_count(timer);
  wait_for_sync(timer);
  timer_set_compare(
      timer,
      (timer->ticks_per_millisecond * timer->active_chunk_milliseconds) - 1U);
  wait_for_sync(timer);
  timer_clear_interrupt_flag(timer);
}

static struct Timer *find_timer_for_interrupt(IRQn_Type interrupt_number)
{
  for (uint8_t i = 0U; i < TIMER_REGISTRY_SIZE; i++)
  {
    if (timer_registry[i] &&
        timer_registry[i]->interrupt_number == interrupt_number)
    {
      return timer_registry[i];
    }
  }

  return NULL;
}

static bool register_timer(struct Timer *timer)
{
  for (uint8_t i = 0U; i < TIMER_REGISTRY_SIZE; i++)
  {
    if (!timer_registry[i] ||
        timer_registry[i]->interrupt_number == timer->interrupt_number)
    {
      timer_registry[i] = timer;
      return true;
    }
  }

  return false;
}

static void unregister_timer(struct Timer *timer)
{
  for (uint8_t i = 0U; i < TIMER_REGISTRY_SIZE; i++)
  {
    if (timer_registry[i] == timer)
    {
      timer_registry[i] = NULL;
      return;
    }
  }
}

static void enable_timer_interrupt(struct Timer *timer)
{
  NVIC_ClearPendingIRQ(timer->interrupt_number);
  timer_enable_compare_interrupt(timer);
  NVIC_EnableIRQ(timer->interrupt_number);
}

static void disable_timer_interrupt(struct Timer *timer)
{
  NVIC_DisableIRQ(timer->interrupt_number);
  timer_disable_compare_interrupt(timer);
}

struct Timer *timer_create(const timer_config_t *config)
{
  if (!config || !config->platform_info)
  {
    return NULL;
  }

  const TimerInfo_SAMD21 *timer_info =
      (const TimerInfo_SAMD21 *)config->platform_info;
  struct Timer *timer = malloc(sizeof(struct Timer));

  if (!timer)
  {
    return NULL;
  }

  timer->timer_counter = timer_info->timer_counter;
  timer->counter_mode = timer_info->counter_mode;
  timer->peripheral_mask = timer_info->peripheral_mask;
  timer->generic_clock_id = timer_info->generic_clock_id;
  timer->prescaler = timer_info->prescaler;
  timer->ticks_per_millisecond = timer_info->ticks_per_millisecond;
  timer->interrupt_number = timer_info->interrupt_number;
  timer->elapsed_milliseconds = 0U;
  timer->remaining_milliseconds = 0U;
  timer->active_chunk_milliseconds = 0U;
  timer->max_chunk_milliseconds = 0U;
  timer->callback = NULL;
  timer->callback_context = NULL;
  timer->callback_active = false;

  if (timer->ticks_per_millisecond == 0U)
  {
    free(timer);
    return NULL;
  }

  timer->max_chunk_milliseconds = timer_max_chunk_milliseconds(timer);

  if (timer->max_chunk_milliseconds == 0U)
  {
    free(timer);
    return NULL;
  }

  if (!register_timer(timer))
  {
    free(timer);
    return NULL;
  }

  PM->APBCMASK.reg |= timer->peripheral_mask;

  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
  {
  }

  GCLK->CLKCTRL.reg =
      timer->generic_clock_id | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;

  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
  {
  }

  timer_reset_counter(timer);
  wait_for_sync(timer);

  timer_configure_counter(timer);
  wait_for_sync(timer);

  timer_set_compare(timer, timer->ticks_per_millisecond - 1U);
  wait_for_sync(timer);

  timer_clear_interrupt_flag(timer);
  timer_disable_compare_interrupt(timer);
  timer_enable_counter(timer);
  wait_for_sync(timer);

  return timer;
}

static void timer_handle_interrupt(struct Timer *timer)
{
  if (!timer)
  {
    return;
  }

  if (!timer_has_compare_interrupt(timer))
  {
    return;
  }

  timer_clear_interrupt_flag(timer);
  timer->elapsed_milliseconds += timer->active_chunk_milliseconds;
  timer->remaining_milliseconds -= timer->active_chunk_milliseconds;

  if (timer->callback_active && timer->remaining_milliseconds == 0U)
  {
    timer_callback_t callback = timer->callback;
    void *context = timer->callback_context;

    timer->callback_active = false;
    disable_timer_interrupt(timer);
    callback(timer, context);
  }
  else if (timer->callback_active)
  {
    timer_schedule_next_chunk(timer);
  }
}

void timer_start(struct Timer *timer, duration_t duration,
                 timer_callback_t callback, void *context)
{
  if (!timer || !callback)
  {
    return;
  }

  if (duration.milliseconds == 0U)
  {
    callback(timer, context);
  }
  else
  {
    timer->elapsed_milliseconds = 0U;
    timer->remaining_milliseconds = duration.milliseconds;
    timer->callback = callback;
    timer->callback_context = context;
    timer->callback_active = true;

    timer_schedule_next_chunk(timer);
    enable_timer_interrupt(timer);
  }
}

static void timer_delay_finished(struct Timer *timer, void *context)
{
  volatile bool *waiting = context;
  *waiting = false;
}

void timer_delay_for(struct Timer *timer, duration_t duration)
{
  if (!timer)
  {
    return;
  }

  volatile bool waiting = true;
  timer_start(timer, duration, timer_delay_finished, (void *)&waiting);

  while (waiting)
  {
    __WFI();
  }
}

void timer_destroy(struct Timer *timer)
{
  if (!timer)
  {
    return;
  }

  disable_timer_interrupt(timer);
  unregister_timer(timer);

  free(timer);
}

void TC3_Handler(void)
{
  timer_handle_interrupt(find_timer_for_interrupt(TC3_IRQn));
}

void TC4_Handler(void)
{
  timer_handle_interrupt(find_timer_for_interrupt(TC4_IRQn));
}

void TC5_Handler(void)
{
  timer_handle_interrupt(find_timer_for_interrupt(TC5_IRQn));
}
