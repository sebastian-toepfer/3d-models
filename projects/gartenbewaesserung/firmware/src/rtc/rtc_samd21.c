#include "rtc.h"
#include "sam.h"

#define RTC_OSC_FREQ 32768UL
#define RTC_PRESCALER_DIV 1024U
#define RTC_TICKS_PER_SEC (RTC_OSC_FREQ / RTC_PRESCALER_DIV)

#define SECONDS_PER_DAY 86400

static void (*user_callback)(void) = 0;

void rtc_set_callback(void (*callback)(void))
{
  user_callback = callback;
}

static inline void rtc_enable_clock_for_rtc()
{
  PM->APBAMASK.reg |= PM_APBAMASK_RTC;
}

static inline void rtc_route_to_osculp32k()
{
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(1);
  GCLK->GENCTRL.reg =
      GCLK_GENCTRL_ID(1) | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_GENEN;
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;
}

static inline void rtc_connect_generator_with_rtc()
{
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(RTC_GCLK_ID) | GCLK_CLKCTRL_GEN_GCLK1 |
                      GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;
}

static void rtc_clock_init(void)
{
  rtc_enable_clock_for_rtc();
  rtc_route_to_osculp32k();
  rtc_connect_generator_with_rtc();
}

static inline void rtc_set_prescaler()
{
  RTC->MODE0.CTRL.reg =
      RTC_MODE0_CTRL_MODE_COUNT32 | RTC_MODE0_CTRL_PRESCALER_DIV1024;
  while (RTC->MODE0.STATUS.bit.SYNCBUSY)
    ;
}

static inline void rtc_reset_count()
{
  RTC->MODE0.COUNT.reg = 0;
  while (RTC->MODE0.STATUS.bit.SYNCBUSY)
    ;
}

static inline void rtc_reset_interrupt_flag()
{
  RTC->MODE0.INTFLAG.reg = RTC_MODE0_INTFLAG_CMP0;
  while (RTC->MODE0.STATUS.bit.SYNCBUSY)
    ;
}

static inline void rtc_configure_interval(uint64_t interval_sec)
{
  RTC->MODE0.COMP[0].reg = interval_sec * RTC_TICKS_PER_SEC;
  ;
  while (RTC->MODE0.STATUS.bit.SYNCBUSY)
    ;
}

static inline void rtc_enable_interrupt()
{
  RTC->MODE0.INTENSET.reg = RTC_MODE0_INTENSET_CMP0;
  NVIC_ClearPendingIRQ(RTC_IRQn);
  NVIC_EnableIRQ(RTC_IRQn);
}

void rtc_enable()
{
  RTC->MODE0.CTRL.reg |= RTC_MODE0_CTRL_ENABLE;
  while (RTC->MODE0.STATUS.bit.SYNCBUSY)
    ;
}

void rtc_disable()
{
  RTC->MODE0.CTRL.reg &= ~RTC_MODE0_CTRL_ENABLE;
  while (RTC->MODE0.STATUS.bit.SYNCBUSY)
    ;
}

void rtc_init_daily_interrupt(void)
{
  rtc_clock_init();
  rtc_disable();
  rtc_set_prescaler();
  rtc_reset_count();
  rtc_reset_interrupt_flag();
  rtc_configure_interval(SECONDS_PER_DAY);
  rtc_enable_interrupt();
  rtc_enable();
}

void RTC_Handler()
{
  if (RTC->MODE0.INTFLAG.bit.CMP0)
  {
    RTC->MODE0.INTFLAG.reg = RTC_MODE0_INTFLAG_CMP0;
    RTC->MODE0.COUNT.reg = 0;
    while (RTC->MODE0.STATUS.bit.SYNCBUSY)
      ;

    if (user_callback)
      user_callback();
  }
}
