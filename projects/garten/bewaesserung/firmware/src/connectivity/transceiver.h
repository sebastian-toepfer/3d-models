#ifndef TRANSCEIVER_H
#define TRANSCEIVER_H
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct Transceiver {
    size_t (*write)(const struct Transceiver *self, const uint8_t *data, size_t len);
    size_t (*read)(const struct Transceiver *self, uint8_t *data, size_t len);
};

size_t transceiver_write(const struct Transceiver *transceiver, const uint8_t *data, size_t len);
size_t transceiver_read(const struct Transceiver *transceiver, uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif
#endif