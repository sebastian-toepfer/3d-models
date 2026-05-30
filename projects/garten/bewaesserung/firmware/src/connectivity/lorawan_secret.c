/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "lorawan_secret.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LORAWAN_DEVICE_EUI_LENGTH 16U
#define LORAWAN_APP_EUI_LENGTH 16U
#define LORAWAN_APP_KEY_LENGTH 32U
#define LORAWAN_SECRET_LENGTH                                                  \
  (LORAWAN_DEVICE_EUI_LENGTH + LORAWAN_APP_EUI_LENGTH + LORAWAN_APP_KEY_LENGTH)

struct LoRaWANSecret
{
  char device_eui[LORAWAN_DEVICE_EUI_LENGTH + 1U];
  char app_eui[LORAWAN_APP_EUI_LENGTH + 1U];
  char app_key[LORAWAN_APP_KEY_LENGTH + 1U];
};

static bool is_hex_char(char value)
{
  return (value >= '0' && value <= '9') || (value >= 'a' && value <= 'f') ||
         (value >= 'A' && value <= 'F');
}

static bool is_hex_string(const char *value, size_t len)
{
  if (!value)
  {
    return false;
  }

  for (size_t i = 0U; i < len; i++)
  {
    if (!is_hex_char(value[i]))
    {
      return false;
    }
  }
  return value[len] == '\0';
}

static void copy_secret_part(char *target, const uint8_t *source, size_t len)
{
  memcpy(target, source, len);
  target[len] = '\0';
}

static bool lorawan_secret_decode(struct LoRaWANSecret *secret,
                                  const uint8_t raw[LORAWAN_SECRET_LENGTH])
{
  copy_secret_part(secret->device_eui, &raw[0], LORAWAN_DEVICE_EUI_LENGTH);
  copy_secret_part(
      secret->app_eui, &raw[LORAWAN_DEVICE_EUI_LENGTH], LORAWAN_APP_EUI_LENGTH);
  copy_secret_part(secret->app_key,
                   &raw[LORAWAN_DEVICE_EUI_LENGTH + LORAWAN_APP_EUI_LENGTH],
                   LORAWAN_APP_KEY_LENGTH);

  return is_hex_string(secret->device_eui, LORAWAN_DEVICE_EUI_LENGTH) &&
         is_hex_string(secret->app_eui, LORAWAN_APP_EUI_LENGTH) &&
         is_hex_string(secret->app_key, LORAWAN_APP_KEY_LENGTH);
}

struct LoRaWANSecret *
lorawan_secret_create_from_store(const struct SecretStore *store)
{
  if (!store)
  {
    return NULL;
  }

  struct LoRaWANSecret *result = malloc(sizeof(struct LoRaWANSecret));
  if (!result)
  {
    return NULL;
  }

  uint8_t raw[LORAWAN_SECRET_LENGTH];
  if (secretstore_read_secret(store, raw, sizeof(raw)) != sizeof(raw) ||
      !lorawan_secret_decode(result, raw))
  {
    free(result);
    return NULL;
  }

  return result;
}

struct LoRaWANJoinConfig *lorawan_secret_create_otaa_join_config_for_device(
    const struct LoRaWANSecret *secret, const char device_eui[17])
{
  if (!secret || !is_hex_string(device_eui, LORAWAN_DEVICE_EUI_LENGTH) ||
      strcmp(secret->device_eui, device_eui) != 0)
  {
    return NULL;
  }

  return lorawan_join_config_create_otaa(secret->app_eui, secret->app_key);
}

void lorawan_secret_destroy(struct LoRaWANSecret *secret)
{
  if (!secret)
  {
    return;
  }

  memset(secret, 0, sizeof(struct LoRaWANSecret));
  free(secret);
}
