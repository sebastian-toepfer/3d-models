/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  struct Transceiver;

  typedef struct
  {
    size_t (*write)(const void *self, const uint8_t *data, size_t len);
    size_t (*read)(const void *self, uint8_t *data, size_t len);
    void (*flush)(const void *self);
  } transceiver_methods_t;

  struct Transceiver *transceiver_create(const void *transport, const transceiver_methods_t *methods);

  size_t transceiver_write(const struct Transceiver *self,
                           const uint8_t *data, size_t len);
  size_t transceiver_read(const struct Transceiver *self, uint8_t *data,
                          size_t len);
  void transceiver_flush(const struct Transceiver *self);

  void transceiver_destroy(struct Transceiver **self);

#ifdef __cplusplus
}
#endif
#endif
