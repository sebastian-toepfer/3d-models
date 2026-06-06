/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef DIGITAL_OUTPUT_PIN_TEST_H
#define DIGITAL_OUTPUT_PIN_TEST_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
#define GPIO_TEST_MAX_EVENTS 64

  typedef enum
  {
    GPIO_TEST_EVENT_CREATE,
    GPIO_TEST_EVENT_ON,
    GPIO_TEST_EVENT_OFF,
    GPIO_TEST_EVENT_TOGGLE,
    GPIO_TEST_EVENT_DESTROY,
  } digital_pin_event_type_t;

  typedef struct
  {
    digital_pin_event_type_t type;
    uint8_t pin_index;
  } digital_pin_event_t;

  typedef struct
  {
    digital_pin_event_t events[GPIO_TEST_MAX_EVENTS];
    size_t count;
    bool overflow;
  } digital_pin_event_log_t;

  typedef struct
  {
    uint8_t pin_index;
    digital_pin_event_log_t *event_log;
  } digital_pin_info_test_t;

  bool
  digital_output_pin_test_has_event(const digital_pin_event_log_t *eventslog,
                                    digital_pin_event_t event);
  size_t digital_output_pin_test_index_of_event(
      const digital_pin_event_log_t *eventslog, digital_pin_event_t event);
  size_t
  digital_output_pin_test_count_events(const digital_pin_event_log_t *eventslog,
                                       digital_pin_event_type_t type);
  void
  digital_output_pin_test_event_log_reset(digital_pin_event_log_t *eventslog);

#ifdef __cplusplus
}
#endif
#endif
