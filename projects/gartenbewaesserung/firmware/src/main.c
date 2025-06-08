#include <Arduino.h>

#include "digital_output_pin_samd21.h"
#include "pinmap.h"
#include "pump/pump.h"

struct Pump *orpu;

// cppcheck-suppress unusedFunction
void setup()
{
  orpu = pump_create(
      &(digital_pin_config_t){
          .pin = PIN_HAUPTRELAIS,
          .platform_info =
              &(DigitalPinInfo_SAMD21){
                  .group =
                      &PORT->Group[g_APinDescription[PIN_HAUPTRELAIS].ulPort],
                  .pin_mask = 1 << g_APinDescription[PIN_HAUPTRELAIS].ulPin}},
      &(digital_pin_config_t){
          .pin = PIN_BEWAESSERUNGSRELAIS,
          .platform_info =
              &(DigitalPinInfo_SAMD21){
                  .group =
                      &PORT->Group[g_APinDescription[PIN_BEWAESSERUNGSRELAIS]
                                       .ulPort],
                  .pin_mask =
                      1 << g_APinDescription[PIN_BEWAESSERUNGSRELAIS].ulPin}},
      &(digital_pin_config_t){
          .pin = PIN_POOLRELAIS,
          .platform_info =
              &(DigitalPinInfo_SAMD21){
                  .group =
                      &PORT->Group[g_APinDescription[PIN_POOLRELAIS].ulPort],
                  .pin_mask = 1 << g_APinDescription[PIN_POOLRELAIS].ulPin}},
      &(timeout_config_t){
          .on_delay = 1000,
          .off_delay = 500,
      });
  if (!orpu)
  {
    struct DigitalOutputPin *led = digital_output_pin_create(&(
        digital_pin_config_t){
        .pin = PIN_HAUPTRELAIS,
        .platform_info = &(DigitalPinInfo_SAMD21){
            .group = &PORT->Group[g_APinDescription[PIN_HAUPTRELAIS].ulPort],
            .pin_mask = 1 << g_APinDescription[PIN_HAUPTRELAIS].ulPin}});
    while (1)
    {
      digital_output_pin_toggle(led);
      delay(250);
    }
  }
}

// cppcheck-suppress unusedFunction
void loop()
{
  // read state from lora ...
  pump_open_garden_valve(orpu);
  delay(5000);
  pump_open_pool_valve(orpu);
  delay(2000);
  pump_close_pool_valve(orpu);
  delay(2000);
  pump_close_garden_valve(orpu);
  delay(1000);
  pump_open_pool_valve(orpu);
  delay(4000);
  pump_close_pool_valve(orpu);
  delay(3000);
}
