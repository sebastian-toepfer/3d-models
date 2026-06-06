/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#include <stddef.h>
#include <stdlib.h>

#include "gpio/samd21/pin_config.h"
#include "gpio/digital_output_pin.h"
#include "mem/mem.h"

struct DigitalOutputPin
{
  PortGroup *group;
  uint32_t pin_mask;
  uint8_t pin_index;
};

static struct DigitalOutputPin *
digital_output_pin_samd21_create(const PinConfig_SAMD21 *pinCfg)
{
  if (!pinCfg)
  {
    return NULL;
  }

  struct DigitalOutputPin *result = mem_allocate(sizeof(struct DigitalOutputPin));
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

struct DigitalOutputPin *
digital_output_pin_create(const pin_config_t *cfg)
{
  if (!cfg || !cfg->platform_config)
  {
    return NULL;
  }
  return digital_output_pin_samd21_create(
      (const PinConfig_SAMD21 *)cfg->platform_config);
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

void digital_output_pin_destroy(struct DigitalOutputPin **pin)
{
  if (!pin || !*pin)
  {
    return;
  }
  mem_free(*pin);
  *pin = NULL;
}