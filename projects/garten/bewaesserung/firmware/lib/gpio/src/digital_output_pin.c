#include "digital_output_pin.h"

#if defined(__SAMD21G18A__) || defined(ARDUINO_SAMD_ZERO)
#include "digital_output_pin_samd21.c"
#include "digital_output_pin_samd21.h"
#include <stdlib.h>

struct DigitalOutputPin *
digital_output_pin_create(const digital_pin_config_t *cfg)
{
  if (!cfg || !cfg->platform_info)
    return NULL;
  return digital_output_pin_samd21_create(
      (const DigitalPinInfo_SAMD21 *)cfg->platform_info);
}

#elif defined(ARDUINO)
#include "digital_output_pin_arduino.c"
#include "digital_output_pin_arduino.h"
#include <stdlib.h>

struct DigitalOutputPin *
digital_output_pin_create(const digital_pin_config_t *cfg)
{
  if (!cfg || !cfg->platform_info)
    return NULL;
  return digital_output_pin_arduino_create((const uint8_t *)&cfg->pin);
}

#else
#error "Unsupported platform: No digital output implementation available"
#endif
