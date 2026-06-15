/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "transceiver.h"
#include "mem/mem.h"

struct Transceiver {
  const void *transport;
  size_t (*write)(const void *self, const uint8_t *data, size_t len);
  size_t (*read)(const void *self, uint8_t *data, size_t len);
  void (*flush)(const void *self);
};

struct Transceiver *transceiver_create(const void *transport, const transceiver_methods_t *methods)
{
  if (!transport || !methods || !methods->write || !methods->read || !methods->flush) {
    return NULL;
  }
  struct Transceiver *result = mem_allocate(sizeof(struct Transceiver));
  if (!result)
  {
    return NULL;
  }
  result->transport = transport;
  result->write = methods->write;
  result->read = methods->read;
  result->flush = methods->flush;
  return result;
}

size_t transceiver_write(const struct Transceiver *self,
                         const uint8_t *data, size_t len)
{
  if (!self || !self->transport || !self->write || !data || len == 0)
  {
    return 0;
  }
  return self->write(self->transport, data, len);
}

size_t transceiver_read(const struct Transceiver *self, uint8_t *data,
                        size_t len)
{
  if (!self || !self->transport || !self->read || !data || len == 0)
  {
    return 0;
  }
  return self->read(self->transport, data, len);
}

void transceiver_flush(const struct Transceiver *self)
{
  if (!self || !self->flush)
  {
    return;
  }
  self->flush(self->transport);
}

void transceiver_destroy(struct Transceiver **self)
{
  if (!self)
  {
    return;
  }
  mem_free(*self);
  *self = NULL;
}
