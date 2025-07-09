/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#ifndef DIGITAL_OUTPUT_PIN_ARDUINO_H
#define DIGITAL_OUTPUT_PIN_ARDUINO_H
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  struct DigitalOutputPin *
  digital_output_pin_arduino_create(const uint8_t *pin);

#ifdef __cplusplus
}
#endif
#endif