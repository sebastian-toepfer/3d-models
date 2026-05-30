/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef LORAWAN_SECRET_H
#define LORAWAN_SECRET_H

#include "lorawan_join_config.h"
#include "secretstore.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct LoRaWANSecret;

  struct LoRaWANSecret *
  lorawan_secret_create_from_store(const struct SecretStore *store);
  struct LoRaWANJoinConfig *lorawan_secret_create_otaa_join_config_for_device(
      const struct LoRaWANSecret *secret, const char device_eui[17]);
  void lorawan_secret_destroy(struct LoRaWANSecret *secret);

#ifdef __cplusplus
}
#endif

#endif
