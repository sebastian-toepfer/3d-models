/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#ifndef DIGITAL_OUTPUT_PIN_SAMD21_H
#define DIGITAL_OUTPUT_PIN_SAMD21_H
#include "sam.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    PortGroup *group;
    uint32_t pin_mask;
    uint8_t pin_index;
  } DigitalPinInfo_SAMD21;

  struct DigitalOutputPin *
  digital_output_pin_samd21_create(const DigitalPinInfo_SAMD21 *cfg);

#ifdef __cplusplus
}
#endif
#endif
