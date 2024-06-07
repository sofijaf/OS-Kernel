//
// Created by os on 1/22/23.
//
#include "../h/syscall_c.hpp"
#include "../lib/mem.h"

void* mem_alloc(size_t size) {
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(0x01));    // MEM_ALLOC
    __asm__ volatile("ecall");

    void* ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int mem_free(void* addr) {
    __asm__ volatile("mv a1, a0");
    __asm__ volatile ("mv a0, %0" : : "r"(0x02));   // MEM_FREE
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

int thread_create(thread_t *handle, void (*start_routine)(void*), void *arg)
{
    // uint64* stack = (uint64*) mem_alloc(DEFAULT_STACK_SIZE);
    uint64* stack = (uint64*) mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
    if(!stack)
        return -3;
    if(!handle)
        return -1;
    if(!start_routine)
        return -2;

    volatile uint64 args[4];
    args[0] = (uint64) handle;
    args[1] = (uint64) start_routine;
    args[2] = (uint64) arg;
    args[3] = (uint64) stack;

    __asm__ volatile ("mv a0, %0" : : "r" (0x11));  // THREAD_CREATE
    __asm__ volatile ("mv a1, %0" : : "r" (args));
    __asm__ volatile ("ecall");

    uint64 volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r" (retval));
    if(retval == 0)
        return 0;
    else
        return -1;
}

int thread_create_not_ready(thread_t *handle, void(*start_routine)(void*), void *arg)
{
    // uint64* stack = (uint64*) mem_alloc(DEFAULT_STACK_SIZE);
    uint64* stack = (uint64*) mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
    if(!stack)
        return -3;
    if(!handle)
        return -1;
    if(!start_routine)
        return -2;

    volatile uint64 args[4];
    args[0] = (uint64) handle;
    args[1] = (uint64) start_routine;
    args[2] = (uint64) arg;
    args[3] = (uint64) stack;

    __asm__ volatile ("mv a0, %0" : : "r" (0x14));  // THREAD_CREATE_NOT_READY
    __asm__ volatile ("mv a1, %0" : : "r" (args));
    __asm__ volatile ("ecall");

    uint64 volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r" (retval));
    if(retval == 0)
        return 0;
    else
        return -1;
}

int thread_start(thread_t handle)
{
    if(!handle)
        return -1;

    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(0x15));    // THREAD_START
    __asm__ volatile ("ecall");

    uint64 volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r" (retval));
    if(retval == 0)
        return 0;
    else
        return -1;
}

void thread_dispatch()
{
    __asm__ volatile ("mv a0, %0" : : "r" (0x13));  // THREAD_DISPATCH
    __asm__ volatile ("ecall");
}

int thread_exit()
{
    __asm__ volatile ("mv a0, %0" : : "r" (0x12));  // THREAD_EXIT
    __asm__ volatile ("ecall");

    uint64 volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r" (retval));
    if(retval == 0)
        return 0;
    else
        return -1;
}

void putc(char c)
{
    __asm__ volatile ("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(0x42));    // PUTC
    __asm__ volatile("ecall");
}

char getc()
{
    __asm__ volatile("mv a0, %0" : : "r"(0x41));    // GETC
    __asm__ volatile("ecall");

    char c;
    __asm__ volatile("mv %0, a0" : "=r"(c));

    return c;
}

int sem_open(sem_t *handle, unsigned int init) {
    if (!handle) {
        return -1;
    }

    volatile uint64 args[2];
    args[0] = (uint64)handle;
    args[1] = init;

    __asm__ volatile("mv a0, %0" : : "r"(0x21));    // SEM_OPEN
    __asm__ volatile("mv a1, %0" : : "r"(args));
    __asm__ volatile("ecall");

    uint64 volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r" (retval));
    if(retval == 0)
        return 0;
    else
        return -1;
}

int sem_close(sem_t handle) {
    if(!handle)
        return -1;

    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(0x22));    // SEM_CLOSE
    __asm__ volatile("ecall");

    uint64 volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r" (retval));
    if(retval == 0)
        return 0;
    else
        return -1;
}

int sem_wait(sem_t id) {
    if(!id)
        return -1;

    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(0x23));    // SEM_WAIT
    __asm__ volatile("ecall");

    uint64 volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r" (retval));
    if(retval == 0)
        return 0;
    else
        return -1;
}

int sem_signal(sem_t id) {
    if(!id)
        return -1;

    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(0x24));    // SEM_SIGNAL
    __asm__ volatile("ecall");

    uint64 volatile retval;
    __asm__ volatile ("mv %0, a0" : "=r" (retval));
    if(retval == 0)
        return 0;
    else
        return -1;
}
