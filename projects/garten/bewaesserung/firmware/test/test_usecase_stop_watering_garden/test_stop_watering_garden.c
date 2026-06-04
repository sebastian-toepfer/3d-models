/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "digital_output_pin.h"
#include "digital_output_pin_test.h"
#include "pump.h"
#include "watering/usecase/stop_watering_garden.h"
#include <unity.h>

static digital_pin_event_log_t events;
static uint8_t main_pin_index = 1;
static struct Pump *pump;
static struct StopWateringGarden *usecase;

void setUp(void)
{
  pump = pump_create(&(pump_config_t){
      .main_switch =
          &(digital_pin_config_t){
              .pin = main_pin_index,
              .platform_info =
                  &(digital_pin_info_test_t){.pin_index = main_pin_index,
                                             .event_log = &events}},
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
              .relay = &(digital_pin_config_t){.pin = 0,
                                               .platform_info =
                                                   &(digital_pin_info_test_t){
                                                       .pin_index = 0,
                                                       .event_log = &events}},
              .lock_relay =
                  &(digital_pin_config_t){
                      .pin = 0,
                      .platform_info =
                          &(digital_pin_info_test_t){.pin_index = 0,
                                                     .event_log = &events}}},
      .delay = duration_create_seconds(1)});
  usecase = stop_wattering_garden_create(pump);
  digital_output_pin_test_event_log_reset(&events);
}

void tearDown(void)
{
  stop_wattering_garden_destroy(&usecase);
  pump_destroy(pump);
}

void should_derefer_pointer()
{
  stop_wattering_garden_destroy(&usecase);
  TEST_ASSERT_FALSE(usecase);
}

void should_stop_wattering_garden()
{
  pump_open_garden_valve(pump);
  stop_wattering_garden_execute(usecase);
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = main_pin_index,
                            .type = GPIO_TEST_EVENT_ON}));
}

int main()
{
  UNITY_BEGIN();

  RUN_TEST(should_derefer_pointer);
  RUN_TEST(should_stop_wattering_garden);

  return UNITY_END();
}
