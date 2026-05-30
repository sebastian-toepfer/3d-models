/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef LORAWAN_JOIN_CONFIG_H
#define LORAWAN_JOIN_CONFIG_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

  struct LoRaWANJoinConfig;

  struct LoRaWANJoinConfig *
  lorawan_join_config_create_otaa(const char app_eui[17],
                                  const char app_key[33]);
  bool
  lorawan_join_config_get_otaa_app_eui(const struct LoRaWANJoinConfig *config,
                                       char app_eui[17]);
  bool
  lorawan_join_config_get_otaa_app_key(const struct LoRaWANJoinConfig *config,
                                       char app_key[33]);
  void lorawan_join_config_destroy(struct LoRaWANJoinConfig *config);

#ifdef __cplusplus
}
#endif

#endif
