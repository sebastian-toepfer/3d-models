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
#include "lora_adapter/lora_adapter.h"
#include "pinmap.h"
#include "pump.h"
#include "rtc/rtc.h"
#include "time/duration.h"
#include "watering/watering.h"

volatile bool lora_daily_beacon_pending = false;
struct Pump *orpu;
struct SecretStore *lora_secrets;
struct LoRa *lori;
struct Watering *watering;
struct LoraAdapter *adapter;

static void lora_daily_beacon()
{
  lora_daily_beacon_pending = true;
}

// cppcheck-suppress unusedFunction
void setup()
{
  orpu = pump_create(
      &(pump_config_t){.main_switch = &Hauptrelais_pin_config,
                       .garden_valve =
                           &(pump_valve_config_t){
                               .relay = &Bewaesserungsrelais_pin_config,
                           },
                       .pool_valve =
                           &(pump_valve_config_t){
                               .relay = &Poolrelais_pin_config,
                               .lock_relay = &Poolvollrelais_pin_config,
                           },
                       .delay = duration_create_seconds(1)});
  watering = watering_create(orpu);
  lora_secrets = eccx08_create(8);
  lori = lora_create(lora_secrets);
  adapter = lora_adapter_create(lori, watering);
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
