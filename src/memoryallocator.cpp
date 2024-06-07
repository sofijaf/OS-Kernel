//
// Created by os on 1/22/23.
//

#include "../h/memoryallocator.hpp"

MemoryDescr *MemoryAllocator::freeMemHead = nullptr;

void* MemoryAllocator::mem_alloc(size_t size)
{
    // poravnanje na MEM_BLOCK_SIZE
    size_t sz;
    if (size % MEM_BLOCK_SIZE == 0)
        sz = size;
    else
        sz = (size / MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE;

    MemoryDescr *blk = freeMemHead;
    MemoryDescr *prev = nullptr;

    // Try to find an existing free block in the list (first fit):
    for (; blk!=nullptr; prev=blk, blk=blk->next)
        if (blk->size >= sz) break;

    // If not found, return nullptr: No free memory
    if (blk == nullptr)
        return nullptr;

    size_t remainingSize = blk->size - sz;

    if (remainingSize >= sizeof(MemoryDescr) + MEM_BLOCK_SIZE) {
        size_t offset = sizeof(MemoryDescr) + sz;
        auto* newBlock = (MemoryDescr*)((char*)blk + offset);
        if (prev) prev->next = newBlock;
        else freeMemHead = newBlock;
        newBlock->next = blk->next;
        newBlock->size = remainingSize - sizeof(MemoryDescr);
    }
    else {
        if (prev) prev->next = blk->next;
        else freeMemHead = blk->next;
    }

    blk->size = sz;
    blk->next = nullptr;

    return (char*)blk + sizeof(MemoryDescr);
}

int MemoryAllocator::mem_free(void* descr)
{
    if (!descr)
        return -1;

    MemoryDescr *newFreeMemDescr = (MemoryDescr*)((char*)descr -  sizeof(MemoryDescr));
    // Ubaci na pocetak liste slobodnih segmenata
    newFreeMemDescr->next = freeMemHead;
    freeMemHead = newFreeMemDescr;

    return 0;
}

void MemoryAllocator::init_memory()
{
    // Na pocetku je cela memorija jedan slobodan segment
    freeMemHead = (MemoryDescr*)HEAP_START_ADDR;

    freeMemHead->next = nullptr;
    freeMemHead->size = (size_t)((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - MEM_BLOCK_SIZE - sizeof(MemoryDescr));
}