/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include <stddef.h>
#include <stdlib.h>

#include "gpio/test/digital_output_pin.h"
#include "gpio/digital_output_pin.h"

struct DigitalOutputPin
{
  uint8_t pin_index;
  digital_pin_event_log_t *event_log;
};

static inline void
digital_output_pin_record(struct DigitalOutputPin *pin,
                          const digital_pin_event_type_t type)
{
  if (pin->event_log->count >= GPIO_TEST_MAX_EVENTS)
  {
    pin->event_log->overflow = true;
    return;
  }
  pin->event_log->events[pin->event_log->count++] =
      (digital_pin_event_t){.type = type, .pin_index = pin->pin_index};
}


static struct DigitalOutputPin *
digital_output_pin_test_create(const digital_pin_info_test_t *pinCfg)
{
  if (!pinCfg)
  {
    return NULL;
  }

  struct DigitalOutputPin *result = malloc(sizeof(struct DigitalOutputPin));
  if (!result)
  {
    return NULL;
  }

  result->pin_index = pinCfg->pin_index;
  result->event_log = pinCfg->event_log;
  digital_output_pin_record(result, GPIO_TEST_EVENT_CREATE);
  return result;
}

struct DigitalOutputPin *
digital_output_pin_create(const pin_config_t *cfg)
{
  if (!cfg || !cfg->platform_config)
  {
    return NULL;
  }
  return digital_output_pin_test_create(
      (const digital_pin_info_test_t *)cfg->platform_config);
}

void digital_output_pin_switch_on(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  digital_output_pin_record(pin, GPIO_TEST_EVENT_ON);
}

void digital_output_pin_switch_off(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  digital_output_pin_record(pin, GPIO_TEST_EVENT_OFF);
}

void digital_output_pin_toggle(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  digital_output_pin_record(pin, GPIO_TEST_EVENT_TOGGLE);
}

void digital_output_pin_destroy(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  digital_output_pin_record(pin, GPIO_TEST_EVENT_DESTROY);
  free(pin);
}

bool digital_output_pin_test_has_event(const digital_pin_event_log_t *eventslog,
                                       digital_pin_event_t event)
{
  if (!eventslog)
  {
    return false;
  }

  for (size_t i = 0; i < eventslog->count; i++)
  {
    if (eventslog->events[i].type == event.type &&
        eventslog->events[i].pin_index == event.pin_index)
    {
      return true;
    }
  }
  return false;
}

size_t
digital_output_pin_test_index_of_event(const digital_pin_event_log_t *eventslog,
                                       digital_pin_event_t event)
{
  if (!eventslog)
  {
    return -1;
  }

  for (size_t i = 0; i < eventslog->count; i++)
  {
    if (eventslog->events[i].type == event.type &&
        eventslog->events[i].pin_index == event.pin_index)
    {
      return i;
    }
  }
  return -1;
}

size_t
digital_output_pin_test_count_events(const digital_pin_event_log_t *eventslog,
                                     digital_pin_event_type_t type)
{
  size_t result = 0;
  if (!eventslog)
  {
    return result;
  }

  for (size_t i = 0; i < eventslog->count; i++)
  {
    if (eventslog->events[i].type == type)
    {
      result++;
    }
  }
  return result;
}

void digital_output_pin_test_event_log_reset(digital_pin_event_log_t *eventslog)
{
  eventslog->count = 0;
  eventslog->overflow = false;
}
