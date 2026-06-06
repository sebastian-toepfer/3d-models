/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include <stdlib.h>

#include "mem/mem.h"

void *mem_allocate(size_t size)
{
    return malloc(size);
}

void mem_free(void *ptr)
{
  if (!ptr)
  {
    return;
  }
  free(ptr);
}
