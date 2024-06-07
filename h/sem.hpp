//
// Created by os on 1/27/23.
//

#ifndef OS1_SEM_HPP
#define OS1_SEM_HPP

#include "list.hpp"
#include "tcb.hpp"
#include "syscall_c.hpp"

class Sem
{
public:
    static Sem* createSem(unsigned init);
    static void deleteSem(sem_t handle);
    ~Sem();

    int wait();
    int signal();

    void* operator new(size_t size) { return MemoryAllocator::mem_alloc(size); }
    void operator delete(void* addr) { MemoryAllocator::mem_free(addr); }

private:
    Sem(unsigned init) : val((int)init) {}
    List<TCB> blockedThreads;

    int val;

    friend class Riscv;
};

#endif //OS1_SEM_HPP
