/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "eccx08_secretstore.h"

#include "time/delay.h"
#include <stddef.h>
#include <stdlib.h>

struct SecretStore
{
  struct ECCX08 *eccx08;
  uint8_t slot;
};

struct SecretStore *eccx08_secretstore_create(struct ECCX08 *eccx08,
                                              uint8_t slot)
{
  if (!eccx08 || slot > ECCX08_MAX_SLOT)
  {
    return NULL;
  }

  struct SecretStore *result = malloc(sizeof(struct SecretStore));
  if (!result)
  {
    return NULL;
  }

  result->eccx08 = eccx08;
  result->slot = slot;
  return result;
}

size_t secretstore_read_secret(const struct SecretStore *store, uint8_t *secret,
                               size_t len)
{
  if (!store || !secret || len == 0U)
  {
    return 0U;
  }

  if (!eccx08_check_access(store->eccx08))
  {
    return 0U;
  }

  if (eccx08_read_slot(store->eccx08, store->slot, secret, len))
  {
    delay_for(duration_create_seconds(1U));
    return len;
  }
  return 0U;
}

size_t secretstore_write_secret(const struct SecretStore *store,
                                const uint8_t *secret, size_t len)
{
  if (!store || !secret || len == 0U)
  {
    return 0U;
  }

  if (!eccx08_check_access(store->eccx08))
  {
    return 0U;
  }

  if (eccx08_write_slot(store->eccx08, store->slot, secret, len))
  {
    delay_for(duration_create_seconds(1U));
    return len;
  }
  return 0U;
}

void secretstore_destroy(struct SecretStore *store)
{
  if (!store)
  {
    return;
  }

  free(store);
}
