/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#ifndef LORA_H
#define LORA_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lorawan_join_config.h"
#include "transceiver.h"

#ifdef __cplusplus
extern "C"
{
#endif

  struct LoRa;
  typedef void (*lora_data_handler_t)(uint8_t *data, size_t len,
                                      void *user_data);

  bool lora_read_device_eui(char device_eui[17]);
  struct LoRa *lora_create(const struct LoRaWANJoinConfig *join_config);
  struct Transceiver *lora_transceiver(const struct LoRa *lora);
  void lora_transceiver_destroy(struct Transceiver *transceiver);
  void lora_poll(const struct LoRa *lora);
  void lora_register_handler(struct LoRa *lora, uint8_t fport,
                             const lora_data_handler_t handler,
                             void *user_data);

#ifdef __cplusplus
}
#endif
#endif
