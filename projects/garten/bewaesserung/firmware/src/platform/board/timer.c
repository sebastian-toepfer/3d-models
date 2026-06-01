#include "time/timer.h"
#include "time/samd21/timer.h"

const timer_config_t default_timer_config = {
    .platform_info =
        &(TimerInfo_SAMD21){.timer_counter = TC5,
                            .counter_mode = TIMER_COUNTER_MODE_16BIT,
                            .peripheral_mask = PM_APBCMASK_TC5,
                            .generic_clock_id = GCLK_CLKCTRL_ID_TC4_TC5,
                            .prescaler = TC_CTRLA_PRESCALER_DIV64,
                            .ticks_per_millisecond = 48000000UL / 64U / 1000U,
                            .interrupt_number = TC5_IRQn}};