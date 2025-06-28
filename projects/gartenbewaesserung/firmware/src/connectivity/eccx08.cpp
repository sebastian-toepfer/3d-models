#include <ArduinoECCX08.h>

#include "eccx08.h"
#include "secretstore.h"

struct SecretStore
{
  uint8_t slot;
};

struct SecretStore *eccx08_create(uint8_t slot)
{
  SecretStore *result = new SecretStore();
  if (!result)
  {
    return NULL;
  }
  result->slot = slot;
  return result;
}

static inline bool eccx08_check_access()
{
  if (!ECCX08.begin())
  {
    return false;
  }

  if (!ECCX08.locked())
  {
    if (!ECCX08.lock())
    {
      return false;
    }
  }
  return true;
}

size_t secretstore_read_secret(const struct SecretStore *store, uint8_t *secret,
                               size_t len)
{
  if (!store || len == 0)
  {
    return 0;
  }

  if (!eccx08_check_access())
  {
    return 0;
  }
  uint8_t result = ECCX08.readSlot(store->slot, secret, len);
  if (result == 1)
  {
    delay(1000);
    return len;
  }
  return 0;
}

size_t secretstore_write_secret(const struct SecretStore *store,
                                const uint8_t *secret, size_t len)
{
  if (!store || len == 0)
  {
    return 0;
  }
  if (!eccx08_check_access())
  {
    return 0;
  }
  size_t result = ECCX08.writeSlot(store->slot, secret, len);
  if (result == 1)
  {
    delay(1000);
    return len;
  }
  return 0;
}