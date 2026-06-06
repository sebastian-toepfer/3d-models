/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include <unity.h>

#include "gpio/test/digital_output_pin.h"
#include "pump.h"
#include "watering/usecase/enable_pool_filling.h"

static digital_pin_event_log_t events;
static uint8_t pool_lock_pin_index = 1;
static struct Pump *pump;
static struct EnablePoolFilling *usecase;

void setUp(void)
{
  pump = pump_create(&(pump_config_t){
      .main_switch =
          &(pin_config_t){.platform_config =
                              &(digital_pin_info_test_t){.pin_index = 0,
                                                         .event_log = &events}},
      .garden_valve =
          &(pump_valve_config_t){
              .relay = &(
                  pin_config_t){.platform_config =
                                    &(digital_pin_info_test_t){
                                        .pin_index = 0, .event_log = &events}},
          },
      .pool_valve =
          &(pump_valve_config_t){
              .relay = &(
                  pin_config_t){.platform_config =
                                    &(digital_pin_info_test_t){
                                        .pin_index = 0, .event_log = &events}},
              .lock_relay =
                  &(pin_config_t){.platform_config =
                                      &(digital_pin_info_test_t){
                                          .pin_index = pool_lock_pin_index,
                                          .event_log = &events}}},
      .delay = duration_create_seconds(1)});
  usecase = enable_pool_filling_create(pump);
  digital_output_pin_test_event_log_reset(&events);
}

void tearDown(void)
{
  enable_pool_filling_destroy(&usecase);
  pump_destroy(&pump);
}

void should_unlock_pool_valve()
{
  pump_lock_pool_valve(pump);
  digital_output_pin_test_event_log_reset(&events);

  enable_pool_filling_execute(usecase);

  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = pool_lock_pin_index,
                            .type = GPIO_TEST_EVENT_ON}));
}

void should_derefer_pointer()
{
  enable_pool_filling_destroy(&usecase);
  TEST_ASSERT_FALSE(usecase);
}

int main()
{
  UNITY_BEGIN();

  RUN_TEST(should_unlock_pool_valve);
  RUN_TEST(should_derefer_pointer);

  return UNITY_END();
}
