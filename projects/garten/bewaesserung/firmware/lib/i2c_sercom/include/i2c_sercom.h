/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef I2C_SERCOM_H
#define I2C_SERCOM_H

#include "sam.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    Sercom *sercom;
    uint32_t peripheral_mask;
    uint16_t core_clock_id;
    uint16_t slow_clock_id;
    PortGroup *sda_group;
    uint8_t sda_pin_index;
    uint8_t sda_pin_mux;
    PortGroup *scl_group;
    uint8_t scl_pin_index;
    uint8_t scl_pin_mux;
    uint32_t baudrate;
    uint32_t rise_time_nanoseconds;
    uint32_t timeout;
  } I2CSercomInfo;

#ifdef __cplusplus
}
#endif
#endif
