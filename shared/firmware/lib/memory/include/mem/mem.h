/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#ifndef MEM_H
#define MEM_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

void *mem_allocate(size_t size);
void mem_free(void *ptr);

#ifdef __cplusplus
}
#endif
#endif