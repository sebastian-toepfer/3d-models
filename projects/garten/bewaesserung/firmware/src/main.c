/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#include <Arduino.h>

#include "board/i2c_peripheral.h"
#include "connectivity/eccx08.h"
#include "connectivity/eccx08_secretstore.h"
#include "connectivity/lora.h"
#include "connectivity/lorawan_join_config.h"
#include "connectivity/lorawan_secret.h"
#include "connectivity/secretstore.h"
#include "connectivity/transceiver.h"
#include "i2c.h"
#include "i2c_peripheral.h"
#include "pinmap.h"
#include "pump.h"
#include "rtc/rtc.h"
#include "time/duration.h"
#include "time/timer.h"

static volatile bool lora_daily_beacon_pending = false;
static struct Pump *orpu;
static struct LoRa *lori;

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

static struct Pump *create_pump()
{
  return pump_create(
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
}

static struct LoRa *create_lora_from_secrets(const struct SecretStore *secrets)
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

static struct LoRa *create_lora_from_eccx08()
{
  struct LoRa *result = NULL;
  struct I2C *i2c = i2c_create(&i2c_eccx08);
  struct I2CPeripheral *eccx08_i2c =
      i2c_peripheral_create(&(i2c_peripheral_config_t){
          .i2c = i2c,
          .address = ECCX08_I2C_ADDRESS,
      });
  if (!eccx08_i2c)
  {
    i2c_destroy(i2c);
    return NULL;
  }

  struct Timer *eccx08_timer = timer_create(&default_timer_config);
  if (!eccx08_timer)
  {
    i2c_peripheral_destroy(eccx08_i2c);
    i2c_destroy(i2c);
    return NULL;
  }

  struct ECCX08 *eccx08 = eccx08_create(
      &(eccx08_config_t){.i2c = eccx08_i2c, .timer = eccx08_timer});
  if (!eccx08)
  {
    timer_destroy(eccx08_timer);
    i2c_peripheral_destroy(eccx08_i2c);
    i2c_destroy(i2c);
    return NULL;
  }

  struct SecretStore *lora_secrets = eccx08_secretstore_create(eccx08, 8);
  result = create_lora_from_secrets(lora_secrets);
  secretstore_destroy(lora_secrets);
  eccx08_destroy(eccx08);
  timer_destroy(eccx08_timer);
  i2c_peripheral_destroy(eccx08_i2c);
  i2c_destroy(i2c);

  return result;
}

static void register_lora_handlers(struct LoRa *lora, struct Pump *pump)
{
  lora_register_handler(lora, 1, handle_garden_valve, pump);
  lora_register_handler(lora, 2, handle_pool_valve, pump);
  lora_register_handler(lora, 3, handle_lock_valves, pump);
}

static void start_lora_daily_beacon()
{
  rtc_set_callback(lora_daily_beacon);
  rtc_init_daily_interrupt();
}

static void start_lora_services(struct LoRa *lora, struct Pump *pump)
{
  register_lora_handlers(lora, pump);
  start_lora_daily_beacon();
}

// cppcheck-suppress unusedFunction
void setup()
{
  orpu = create_pump();
  if (!orpu)
  {
    return;
  }

  lori = create_lora_from_eccx08();
  if (!lori)
  {
    return;
  }

  start_lora_services(lori, orpu);
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
