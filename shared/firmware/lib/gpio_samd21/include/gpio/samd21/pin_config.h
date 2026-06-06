/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#ifndef DIGITAL_OUTPUT_PIN_SAMD21_H
#define DIGITAL_OUTPUT_PIN_SAMD21_H
#include <stdint.h>
#include <sam.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    PortGroup *group;
    uint32_t pin_mask;
    uint8_t pin_index;
  } PinConfig_SAMD21;

#ifdef __cplusplus
}
#endif
#endif