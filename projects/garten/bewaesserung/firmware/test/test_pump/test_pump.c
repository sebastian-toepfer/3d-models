#include <unity.h>

#include "gpio/test/digital_output_pin.h"
#include "pump.h"
#include "time/duration.h"

static digital_pin_event_log_t events;

enum
{
  MAIN_SWITCH_PIN_INDEX = 1,
  GARDEN_VALVE_RELAY_PIN_INDEX = 2,
  POOL_VALVE_RELAY_PIN_INDEX = 3,
  POOL_VALVE_LOCK_RELAY_PIN_INDEX = 4,
};

static struct Pump *create_test_pump()
{
  return pump_create(&(pump_config_t){
      .main_switch = &(pin_config_t){.platform_config =
                                         &(digital_pin_info_test_t){
                                             .pin_index = MAIN_SWITCH_PIN_INDEX,
                                             .event_log = &events}},
      .garden_valve =
          &(pump_valve_config_t){
              .relay = &(pin_config_t){.platform_config =
                                           &(digital_pin_info_test_t){
                                               .pin_index =
                                                   GARDEN_VALVE_RELAY_PIN_INDEX,
                                               .event_log = &events}},
          },
      .pool_valve =
          &(pump_valve_config_t){
              .relay = &(
                  pin_config_t){.platform_config =
                                    &(digital_pin_info_test_t){
                                        .pin_index = POOL_VALVE_RELAY_PIN_INDEX,
                                        .event_log = &events}},
              .lock_relay =
                  &(pin_config_t){
                      .platform_config =
                          &(digital_pin_info_test_t){
                              .pin_index = POOL_VALVE_LOCK_RELAY_PIN_INDEX,
                              .event_log = &events}}},
      .delay = duration_create_seconds(1)});
}

void setUp()
{
  digital_output_pin_test_event_log_reset(&events);
}

void tearDown()
{
}

void should_create_all_pins()
{
  struct Pump *pump = create_test_pump();

  TEST_ASSERT_EQUAL(
      4, digital_output_pin_test_count_events(&events, GPIO_TEST_EVENT_CREATE));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_CREATE}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = GARDEN_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_CREATE}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_CREATE}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_LOCK_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_CREATE}));

  pump_destroy(&pump);
}

void should_destroy_all_pins()
{
  struct Pump *pump = create_test_pump();
  digital_output_pin_test_event_log_reset(&events);
  pump_destroy(&pump);

  TEST_ASSERT_EQUAL(
      4,
      digital_output_pin_test_count_events(&events, GPIO_TEST_EVENT_DESTROY));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_DESTROY}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = GARDEN_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_DESTROY}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_DESTROY}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_LOCK_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_DESTROY}));
  TEST_ASSERT_FALSE(pump);
}

void should_open_garden_valve()
{
  struct Pump *pump = create_test_pump();
  digital_output_pin_test_event_log_reset(&events);

  pump_open_garden_valve(pump);

  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = GARDEN_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = GARDEN_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));

  pump_destroy(&pump);
}

void should_close_garden_valve()
{
  struct Pump *pump = create_test_pump();
  pump_open_garden_valve(pump);
  digital_output_pin_test_event_log_reset(&events);

  pump_close_garden_valve(pump);

  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));

  pump_destroy(&pump);
}

void should_created_with_locked_pool_valve()
{
  struct Pump *pump = create_test_pump();
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_LOCK_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));

  pump_destroy(&pump);
}

void should_open_pool_valve()
{
  struct Pump *pump = create_test_pump();
  digital_output_pin_test_event_log_reset(&events);

  pump_open_pool_valve(pump);

  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));

  pump_destroy(&pump);
}

void should_close_pool_valve()
{
  struct Pump *pump = create_test_pump();
  pump_open_pool_valve(pump);
  digital_output_pin_test_event_log_reset(&events);

  pump_close_pool_valve(pump);

  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));

  pump_destroy(&pump);
}

void should_reopen_garden_valve_if_both_are_open_and_pool_should_closed()
{
  struct Pump *pump = create_test_pump();
  pump_open_pool_valve(pump);
  pump_open_garden_valve(pump);
  digital_output_pin_test_event_log_reset(&events);

  pump_close_pool_valve(pump);

  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = GARDEN_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = GARDEN_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));

  pump_destroy(&pump);
}

void should_reopen_pool_valve_if_both_are_open_and_garden_should_closed()
{
  struct Pump *pump = create_test_pump();
  pump_open_pool_valve(pump);
  pump_open_garden_valve(pump);
  digital_output_pin_test_event_log_reset(&events);

  pump_close_garden_valve(pump);

  TEST_ASSERT_EQUAL(4, events.count);
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));

  pump_destroy(&pump);
}

void should_lock_pool_valve()
{
  struct Pump *pump = create_test_pump();
  digital_output_pin_test_event_log_reset(&events);

  pump_lock_pool_valve(pump);

  TEST_ASSERT_EQUAL(1, events.count);
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_LOCK_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));

  pump_destroy(&pump);
}

void should_close_pool_valve_before_locking()
{
  struct Pump *pump = create_test_pump();
  pump_open_pool_valve(pump);
  digital_output_pin_test_event_log_reset(&events);

  pump_lock_pool_valve(pump);

  TEST_ASSERT_EQUAL(3, events.count);
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = MAIN_SWITCH_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_LOCK_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_OFF}));

  pump_destroy(&pump);
}

void should_unlock_pool_valve()
{
  struct Pump *pump = create_test_pump();
  pump_lock_pool_valve(pump);
  digital_output_pin_test_event_log_reset(&events);

  pump_unlock_pool_valve(pump);

  TEST_ASSERT_EQUAL(1, events.count);
  TEST_ASSERT_TRUE(digital_output_pin_test_has_event(
      &events,
      (digital_pin_event_t){.pin_index = POOL_VALVE_LOCK_RELAY_PIN_INDEX,
                            .type = GPIO_TEST_EVENT_ON}));

  pump_destroy(&pump);
}

int main()
{
  UNITY_BEGIN();

  RUN_TEST(should_create_all_pins);
  RUN_TEST(should_open_garden_valve);
  RUN_TEST(should_close_garden_valve);

  RUN_TEST(should_created_with_locked_pool_valve);
  RUN_TEST(should_open_pool_valve);
  RUN_TEST(should_close_pool_valve);

  RUN_TEST(should_reopen_garden_valve_if_both_are_open_and_pool_should_closed);
  RUN_TEST(should_reopen_pool_valve_if_both_are_open_and_garden_should_closed);

  RUN_TEST(should_lock_pool_valve);
  RUN_TEST(should_close_pool_valve_before_locking);

  RUN_TEST(should_unlock_pool_valve);

  RUN_TEST(should_destroy_all_pins);

  return UNITY_END();
}
