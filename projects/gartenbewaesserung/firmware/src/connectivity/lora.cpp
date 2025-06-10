#include <MKRWAN.h>
#include <stddef.h>

#include "lora.h"
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

struct LoRa *lora_create(const struct SecretStore *lora_secrets)
{
  struct LoRa *result = new LoRa();
  if (!result)
  {
    return NULL;
  }

  if (!modem.begin(EU868))
  {
    delete result;
    return NULL;
  }
  uint8_t secret[64];
  size_t read = secretstore_read_secret(lora_secrets, secret, 64);
  if (read != 64)
  {
    delete result;
    return NULL;
  }

  char appEui[17];
  char appKey[33];
  for (unsigned int i = 0; i < 16; i++)
  {
    if (modem.deviceEUI().charAt(i) != secret[i])
    {
      delete result;
      return NULL;
    }
  }
  for (unsigned int i = 0; i < 16; i++)
  {
    appEui[i] = secret[i + 16];
  }
  appEui[16] = '\0';
  for (unsigned int i = 0; i < 32; i++)
  {
    appKey[i] = secret[i + 32];
  }
  appKey[32] = '\0';

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
  uint8_t avail = modem.available();
  if (!lora || avail == 0)
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
  struct Transceiver *result = new Transceiver();
  if (!result)
  {
    return NULL;
  }
  result->write = lorawan_write;
  result->read = lorawan_read;
  return result;
}