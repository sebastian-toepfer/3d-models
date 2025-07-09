/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#include "digital_output_pin_samd21.h"
#include "digital_output_pin.h"
#include <stddef.h>
#include <stdlib.h>

struct DigitalOutputPin
{
  PortGroup *group;
  uint32_t pin_mask;
  uint8_t pin_index;
};

struct DigitalOutputPin *
digital_output_pin_samd21_create(const DigitalPinInfo_SAMD21 *pinCfg)
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

  result->group = pinCfg->group;
  result->pin_mask = pinCfg->pin_mask;
  result->pin_index = pinCfg->pin_index;

  result->group->DIRSET.reg = result->pin_mask;
  result->group->OUTCLR.reg = result->pin_mask;
  result->group->PINCFG[result->pin_index].reg = PORT_PINCFG_INEN;

  return result;
}

void digital_output_pin_switch_on(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  pin->group->OUTSET.reg = pin->pin_mask;
}

void digital_output_pin_switch_off(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  pin->group->OUTCLR.reg = pin->pin_mask;
}

void digital_output_pin_toggle(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  pin->group->OUTTGL.reg = pin->pin_mask;
}

void digital_output_pin_destroy(struct DigitalOutputPin *pin)
{
  if (!pin)
  {
    return;
  }
  free(pin);
}