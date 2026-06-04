/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "digital_output_pin.h"
#include "digital_output_pin_test.h"
#include "pump.h"
#include "watering/usecase/start_fill_pool.h"
#include <unity.h>

static digital_pin_event_log_t events;
static uint8_t pool_valve_pin_index = 1;
static struct Pump *pump;
static struct StartFillPool *usecase;

void setUp(void)
{
  pump = pump_create(&(pump_config_t){
      .main_switch = &(
          digital_pin_config_t){.pin = 0,
                                .platform_info =
                                    &(digital_pin_info_test_t){
                                        .pin_index = 0, .event_log = &events}},
      .garden_valve =
          &(pump_valve_config_t){
              .relay = &(digital_pin_config_t){.pin = 0,
                                               .platform_info =
                                                   &(digital_pin_info_test_t){
                                                       .pin_index = 0,
                                                       .event_log = &events}},
          },
      .pool_valve =
          &(pump_valve_config_t){
              .relay = &(digital_pin_config_t){.pin = pool_valve_pin_index,
                                               .platform_info =
                                                   &(digital_pin_info_test_t){
                                                       .pin_index =
                                                           pool_valve_pin_index,
                                                       .event_log = &events}},
              .lock_relay =
                  &(digital_pin_config_t){
                      .pin = 0,
                      .platform_info =
                          &(digital_pin_info_test_t){.pin_index = 0,
                                                     .event_log = &events}}},
      .delay = duration_create_seconds(1)});
  usecase = start_fill_pool_create(pump);
  digital_output_pin_test_event_log_reset(&events);
}

void tearDown(void)
{
  start_fill_pool_destroy(&usecase);
  pump_destroy(pump);
}

void should_derefer_pointer()
{
  start_fill_pool_destroy(&usecase);
  TEST_ASSERT_FALSE(usecase);
}

void should_open_pool_valve()
{
  start_fill_pool_execute(usecase);
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = pool_valve_pin_index,
                            .type = GPIO_TEST_EVENT_ON}));
}

int main()
{
  UNITY_BEGIN();

  RUN_TEST(should_derefer_pointer);
  RUN_TEST(should_open_pool_valve);

  return UNITY_END();
}
