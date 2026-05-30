/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#include <Arduino.h>

#include "connectivity/eccx08.h"
#include "connectivity/lora.h"
#include "connectivity/lorawan_join_config.h"
#include "connectivity/lorawan_secret.h"
#include "connectivity/secretstore.h"
#include "connectivity/transceiver.h"
#include "digital_output_pin_samd21.h"
#include "pinmap.h"
#include "pump.h"
#include "rtc/rtc.h"
#include "time/duration.h"

volatile bool lora_daily_beacon_pending = false;
struct Pump *orpu;
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

/*
  0x00 - 0000 0000 -> unlocked all
  0x01 - 0000 0001 -> lock garden
  0x02 - 0000 0010 -> lock pool
  0x03 - 0000 0011 -> lock garden & lock pool
  ...
  0xFF - 1111 1111 -> lock all
  */
static void handle_lock_valves(uint8_t *data, size_t len, void *pump)
{
  if (len <= 0)
  {
    return;
  }

  if (data[0] & 1)
  {
    pump_lock_garden_valve((struct Pump *)pump);
  }
  else
  {
    pump_unlock_garden_valve((struct Pump *)pump);
  }

  if (data[0] & (1 << 1))
  {
    pump_lock_pool_valve((struct Pump *)pump);
  }
  else
  {
    pump_unlock_pool_valve((struct Pump *)pump);
  }
}

static void lora_daily_beacon()
{
  lora_daily_beacon_pending = true;
}

static struct LoRa *create_lora(const struct SecretStore *secrets)
{
  struct LoRa *result = NULL;
  struct LoRaWANSecret *lora_secret = lorawan_secret_create_from_store(secrets);
  char device_eui[17];
  struct LoRaWANJoinConfig *join_config = NULL;

  if (lora_secret && lora_read_device_eui(device_eui))
  {
    join_config = lorawan_secret_create_otaa_join_config_for_device(lora_secret,
                                                                    device_eui);
  }

  if (join_config)
  {
    result = lora_create(join_config);
  }

  lorawan_join_config_destroy(join_config);
  lorawan_secret_destroy(lora_secret);

  return result;
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
  struct SecretStore *lora_secrets = eccx08_create(8);
  lori = create_lora(lora_secrets);
  secretstore_destroy(lora_secrets);

  if (!lori)
  {
    return;
  }

  lora_register_handler(lori, 1, handle_garden_valve, orpu);
  lora_register_handler(lori, 2, handle_pool_valve, orpu);
  lora_register_handler(lori, 3, handle_lock_valves, orpu);

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
