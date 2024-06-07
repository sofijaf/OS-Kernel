//
// Created by os on 1/22/23.
//

#ifndef OS1_MEMORYALLOCATOR_HPP
#define OS1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

/*
 * Ideja je da ulancavam slobodne blokove - FIRST FIT algoritam
 * Cim neki blok zelim da alociram, izbacujem ga iz liste slobodnih i vracam ga kao povratnu vrednost malloc
 * Kad radim free, dati blok samo ubacujem na pocetak liste
 */

struct MemoryDescr
{
    size_t size;
    MemoryDescr *next;
};

class MemoryAllocator
{
public:
    // pokazivac na prvi slobodan blok
    static MemoryDescr *freeMemHead;

    static void* mem_alloc(size_t size);

    static int mem_free(void* descr);

    static void init_memory();
};

#endif //OS1_MEMORYALLOCATOR_HPP
