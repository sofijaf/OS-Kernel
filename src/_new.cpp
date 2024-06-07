//
// Created by os on 1/4/23.
//

#include "../lib/mem.h"
#include "../h/memoryallocator.hpp"
#include "../h/syscall_c.hpp"

void *operator new(uint64 n)
{
    return mem_alloc(n);
}

void *operator new[](uint64 n)
{
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    mem_free(p);
}

void operator delete[](void *p) noexcept
{
    mem_free(p);
}