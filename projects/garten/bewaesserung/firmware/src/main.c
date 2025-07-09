/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#include <Arduino.h>

#include "connectivity/eccx08.h"
#include "connectivity/lora.h"
#include "connectivity/secretstore.h"
#include "connectivity/transceiver.h"
#include "digital_output_pin_samd21.h"
#include "pinmap.h"
#include "pump/pump.h"
#include "rtc/rtc.h"

volatile bool lora_daily_beacon_pending = false;
struct Pump *orpu;
struct SecretStore *lora_secrets;
struct LoRa *lori;

static void handle_garden_valve(uint8_t *data, size_t len, void *pump)
{
  if (len > 0 && data[0] == 0x01)
  {
    pump_open_garden_valve((struct Pump *)pump);
  }
  else
  {
    pump_close_garden_valve((struct Pump *)pump);
  }
}

static void handle_pool_valve(uint8_t *data, size_t len, void *pump)
{
  if (len > 0 && data[0] == 0x01)
  {
    pump_open_pool_valve((struct Pump *)pump);
  }
  else
  {
    pump_close_pool_valve((struct Pump *)pump);
  }
}

static void lora_daily_beacon()
{
  lora_daily_beacon_pending = true;
}

// cppcheck-suppress unusedFunction
void setup()
{
  orpu = pump_create(&Hauptrelais_pin_config,
                     &Bewaesserungsrelais_pin_config,
                     &Poolrelais_pin_config,
                     &(timeout_config_t){
                         .on_delay = 1000,
                         .off_delay = 500,
                     });

  lora_secrets = eccx08_create(8);
  lori = lora_create(lora_secrets);
  lora_register_handler(lori, 1, handle_garden_valve, orpu);
  lora_register_handler(lori, 2, handle_pool_valve, orpu);

  rtc_set_callback(lora_daily_beacon);
  rtc_init_daily_interrupt();
}

// cppcheck-suppress unusedFunction
void loop()
{
  lora_poll(lori);
  if (lora_daily_beacon_pending)
  {
    struct Transceiver *tx = lora_transceiver(lori);
    if (transceiver_write(tx, &(uint8_t){0}, 1))
    {
      lora_daily_beacon_pending = false;
    }
    lora_transceiver_destroy(tx);
  }
}
