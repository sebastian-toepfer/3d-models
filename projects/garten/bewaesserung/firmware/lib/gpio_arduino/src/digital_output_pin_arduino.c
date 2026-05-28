/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#include "digital_output_pin.h"
#include <Arduino.h>
#include <stdlib.h>

struct DigitalOutputPin
{
  uint8_t pin;
};

struct DigitalOutputPin *
digital_output_pin_create(const digital_pin_config_t *cfg)
{
  if (!cfg || !cfg->platform_info)
  {
    return NULL;
  }
  return digital_output_pin_arduino_create((const uint8_t *)&cfg->pin);
}

struct DigitalOutputPin *digital_output_pin_arduino_create(const uint8_t *pin)
{
  if (!pin)
  {
    return NULL;
  }
  struct DigitalOutputPin *result = malloc(sizeof(struct DigitalOutputPin));
  if (!result)
  {
    return NULL;
  }
  result->pin = *pin;
  pinMode(result->pin, OUTPUT);
  digital_output_pin_switch_off(result);
  return result;
}

void digital_output_pin_switch_on(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  digitalWrite(pin->pin, HIGH);
}

void digital_output_pin_switch_off(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  digitalWrite(pin->pin, LOW);
}

void digital_output_pin_toggle(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  digitalWrite(pin->pin, !digitalRead(pin->pin));
}

void digital_output_pin_destroy(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  free(pin);
}
