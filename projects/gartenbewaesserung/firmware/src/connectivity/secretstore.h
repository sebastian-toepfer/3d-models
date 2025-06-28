#ifndef SECURITYSTORE_H
#define SECURITYSTORE_H
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct SecretStore;

size_t secretstore_read_secret(const struct SecretStore *store, uint8_t *secret, size_t len);
size_t secretstore_write_secret(const struct SecretStore *store, const uint8_t *secret, size_t len);

#ifdef __cplusplus
}
#endif
#endif