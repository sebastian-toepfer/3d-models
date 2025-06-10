#include <Arduino.h>

#include "connectivity/eccx08.h"
#include "connectivity/lora.h"
#include "connectivity/secretstore.h"
#include "connectivity/transceiver.h"
#include "digital_output_pin_samd21.h"
#include "pinmap.h"
#include "pump/pump.h"

struct Pump *orpu;
struct SecretStore *lora_secrets;
struct Transceiver *lori;

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

  lora_secrets = eccx08_create(8);
  lori = lora_create(lora_secrets);
}

// cppcheck-suppress unusedFunction
void loop()
{
  uint8_t garden_valve_state[1];
  if (transceiver_read(lori, garden_valve_state, 1) == 1)
  {
    if (garden_valve_state[0] == 0x01)
    {
      pump_open_garden_valve(orpu);
    }
    else
    {
      pump_close_garden_valve(orpu);
    }
  }
  // das andere relais ... evtl. port?
  // sollten wir hier mal ueber IRQ nachdenken?
}
