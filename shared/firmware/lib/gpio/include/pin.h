/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#ifndef PIN_H
#define PIN_H
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    uint8_t pin;
    void *platform_info;
  } digital_pin_config_t;

#ifdef __cplusplus
}
#endif
#endif
