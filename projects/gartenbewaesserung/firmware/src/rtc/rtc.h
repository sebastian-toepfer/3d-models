#ifndef RTC_H
#define RTC_H

#ifdef __cplusplus
extern "C"
{
#endif

  void rtc_init_daily_interrupt();
  void rtc_enable();
  void rtc_disable();
  void rtc_set_callback(void (*callback)());

#ifdef __cplusplus
}
#endif
#endif