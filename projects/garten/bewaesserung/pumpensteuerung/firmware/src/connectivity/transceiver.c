/*
 * MIT License
 * Copyright (c) 2025 Sebastian Toepfer
 */
#include "transceiver.h"

size_t transceiver_write(const struct Transceiver *transceiver,
                         const uint8_t *data, size_t len)
{
  if (!transceiver || !data || len == 0)
  {
    return 0;
  }
  return transceiver->write(transceiver, data, len);
}

size_t transceiver_read(const struct Transceiver *transceiver, uint8_t *data,
                        size_t len)
{
  if (!transceiver || !data || len == 0)
  {
    return 0;
  }
  return transceiver->read(transceiver, data, len);
}