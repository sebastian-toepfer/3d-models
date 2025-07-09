/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#ifndef DIGITAL_OUTPUT_PIN_H
#define DIGITAL_OUTPUT_PIN_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  struct DigitalOutputPin;

  // General-purpose configuration for any platform
  typedef struct
  {
    uint8_t pin;
    void *platform_info;
  } digital_pin_config_t;

  // Create a new digital output pin object
  struct DigitalOutputPin *
  digital_output_pin_create(const digital_pin_config_t *config);
  // Set pin to HIGH
  void digital_output_pin_switch_on(struct DigitalOutputPin *pin);
  // Set pin to LOW
  void digital_output_pin_switch_off(struct DigitalOutputPin *pin);
  // Toggle the current pin state
  void digital_output_pin_toggle(struct DigitalOutputPin *pin);
  // Destroy the digital output pin object
  void digital_output_pin_destroy(struct DigitalOutputPin *pin);

#ifdef __cplusplus
}
#endif
#endif