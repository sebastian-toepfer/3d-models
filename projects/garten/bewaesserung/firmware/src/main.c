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
#include "pump.h"
#include "rtc/rtc.h"
#include "time/duration.h"
#include "watering/watering.h"

volatile bool lora_daily_beacon_pending = false;
struct Pump *orpu;
struct SecretStore *lora_secrets;
struct LoRa *lori;
struct Watering *watering;

static void handle_garden_valve(uint8_t *data, size_t len, void *pump)
{
  struct Watering *curWatering = context;
  if (len > 0 && data[0] == 0x01)
  {
    watering_start_watering_garden(curWatering);
  }
  else
  {
    watering_stop_watering_garden(curWatering);
  }
}

static void handle_pool_valve(uint8_t *data, size_t len, void *context)
{
  struct Watering *curWatering = context;
  if (len > 0 && data[0] == 0x01)
  {
    watering_start_fill_pool(curWatering);
  }
  else
  {
    watering_stop_fill_pool(curWatering);
  }
}

/*
  0x00 - 0000 0000 -> unlocked all (same as unlock pool)
  0x01 - 0000 0001 -> lock garden (not supported yet)
  0x02 - 0000 0010 -> lock pool
  0x03 - 0000 0011 -> lock garden (not supported yet) & lock pool
  ...
  0xFF - 1111 1111 -> lock all
  */
static void handle_lock_valves(uint8_t *data, size_t len, void *context)
{
  if (len <= 0)
  {
    return;
  }

  struct Watering *curWatering = context;
  if (data[0] & (1 << 1))
  {
    watering_disable_pool_filling(curWatering);
  }
  else
  {
    watering_enable_pool_filling(curWatering);
  }
}

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
  lora_register_handler(lori, 1, handle_garden_valve, watering);
  lora_register_handler(lori, 2, handle_pool_valve, watering);
  lora_register_handler(lori, 3, handle_lock_valves, watering);

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
