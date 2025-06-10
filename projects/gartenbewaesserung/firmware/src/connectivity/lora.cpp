#include <MKRWAN.h>
#include <stddef.h>

#include "lora.h"
#include "transceiver.h"

LoRaModem modem;

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

static size_t lorawan_read(const struct Transceiver *transceiver, uint8_t *data,
                           size_t len)
{
  if (!modem.available())
  {
    return 0;
  }
  return modem.readBytes(data, len);
}

struct Transceiver *lora_create(const struct SecretStore *lora_secrets)
{
  struct Transceiver *result = new Transceiver();
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
  result->write = lorawan_write;
  result->read = lorawan_read;
  return result;
}