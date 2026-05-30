/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "lorawan_join_config.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct LoRaWANJoinConfig
{
  char app_eui[17];
  char app_key[33];
};

struct LoRaWANJoinConfig *
lorawan_join_config_create_otaa(const char app_eui[17], const char app_key[33])
{
  if (!app_eui || !app_key)
  {
    return NULL;
  }

  struct LoRaWANJoinConfig *result = malloc(sizeof(struct LoRaWANJoinConfig));
  if (!result)
  {
    return NULL;
  }

  memcpy(result->app_eui, app_eui, sizeof(result->app_eui));
  memcpy(result->app_key, app_key, sizeof(result->app_key));
  return result;
}

bool lorawan_join_config_get_otaa_app_eui(
    const struct LoRaWANJoinConfig *config, char app_eui[17])
{
  if (!config || !app_eui)
  {
    return false;
  }

  memcpy(app_eui, config->app_eui, sizeof(config->app_eui));
  return true;
}

bool lorawan_join_config_get_otaa_app_key(
    const struct LoRaWANJoinConfig *config, char app_key[33])
{
  if (!config || !app_key)
  {
    return false;
  }

  memcpy(app_key, config->app_key, sizeof(config->app_key));
  return true;
}

void lorawan_join_config_destroy(struct LoRaWANJoinConfig *config)
{
  if (!config)
  {
    return;
  }

  memset(config, 0, sizeof(struct LoRaWANJoinConfig));
  free(config);
}
