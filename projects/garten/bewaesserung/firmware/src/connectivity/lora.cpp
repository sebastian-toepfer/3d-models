#include <MKRWAN.h>
#include <stddef.h>

#include "lora.h"
#include "lorawan_join_config.h"
#include "transceiver.h"

#define LORA_MAX_PORT 224
#define LORA_MAX_MSG_LEN 242

typedef struct
{
  lora_data_handler_t handler;
  void *user_data;
} LoraDataHandlerEntry;

struct LoRa
{
  LoraDataHandlerEntry handlers[LORA_MAX_PORT];
  // chatgpt: LoRaModem *modem; nur fuer was.
};

static LoRaModem modem;
static bool modem_started = false;

static bool lora_begin_modem()
{
  if (modem_started)
  {
    return true;
  }

  if (!modem.begin(EU868))
  {
    return false;
  }

  modem_started = true;
  return true;
}

bool lora_read_device_eui(char device_eui[17])
{
  if (!device_eui || !lora_begin_modem())
  {
    return false;
  }

  for (unsigned int i = 0; i < 16; i++)
  {
    device_eui[i] = modem.deviceEUI().charAt(i);
  }
  device_eui[16] = '\0';
  return true;
}

struct LoRa *lora_create(const struct LoRaWANJoinConfig *join_config)
{
  if (!join_config)
  {
    return NULL;
  }

  struct LoRa *result = new LoRa();
  if (!result)
  {
    return NULL;
  }

  if (!lora_begin_modem())
  {
    delete result;
    return NULL;
  }

  char appEui[17];
  char appKey[33];
  if (!lorawan_join_config_get_otaa_app_eui(join_config, appEui) ||
      !lorawan_join_config_get_otaa_app_key(join_config, appKey))
  {
    delete result;
    return NULL;
  }

  modem.configureClass(CLASS_C);
  modem.setADR(true);
  if (!modem.joinOTAA(appEui, appKey))
  {
    delete result;
    return NULL;
  }
  modem.minPollInterval(1);
  modem.poll();
  return result;
}

static size_t lorawan_write(const struct Transceiver *tranceiver,
                            const uint8_t *data, size_t len)
{
  if (!tranceiver || !data || len == 0)
  {
    return 0;
  }
  modem.beginPacket();
  modem.write(data, len);
  return modem.endPacket(true);
}

void lora_register_handler(struct LoRa *lora, uint8_t fport,
                           const lora_data_handler_t handler, void *user_data)
{
  if (!lora || fport == 0 || fport >= LORA_MAX_PORT)
  {
    return;
  }

  lora->handlers[fport].handler = handler;
  lora->handlers[fport].user_data = user_data;
}

void lora_poll(const struct LoRa *lora)
{
  if (!lora)
  {
    return;
  }

  uint8_t avail = modem.available();
  if (avail == 0)
  {
    return;
  }
  uint8_t buf[LORA_MAX_MSG_LEN];
  size_t len = modem.readBytes(buf, min(avail, sizeof(buf)));
  if (len == 0)
  {
    return;
  }

  uint8_t port = modem.getDownlinkPort();
  if (port >= LORA_MAX_PORT)
  {
    return;
  }

  const LoraDataHandlerEntry *entry = &lora->handlers[port];
  if (!entry || !entry->handler)
  {
    return;
  }

  uint8_t *msg = (uint8_t *)malloc(len);
  if (!msg)
  {
    return;
  }
  memcpy(msg, buf, len);
  entry->handler(msg, len, entry->user_data);
  free(msg);
}

static size_t lorawan_read(const struct Transceiver *transceiver, uint8_t *data,
                           size_t len)
{
  if (!modem.available())
  {
    return 0;
  }
  return modem.readBytes(data, len);
}

struct Transceiver *lora_transceiver(const struct LoRa *lora)
{
  if (!lora)
  {
    return NULL;
  }

  struct Transceiver *result = new Transceiver();
  if (!result)
  {
    return NULL;
  }
  result->write = lorawan_write;
  result->read = lorawan_read;
  return result;
}

void lora_transceiver_destroy(struct Transceiver *transceiver)
{
  if (!transceiver)
  {
    return;
  }
  delete transceiver;
}
