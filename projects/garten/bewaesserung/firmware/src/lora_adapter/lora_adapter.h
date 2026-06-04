/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "connectivity/lora.h"
#include "watering/watering.h"

struct LoraAdapter;

struct LoraAdapter *lora_adapter_create(struct LoRa *lora,
                                        struct Watering *watering);
void lora_adapter_destroy(struct LoraAdapter **self);