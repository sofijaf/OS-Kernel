//
// Created by os on 1/22/23.
//

#ifndef OS1_SYSCALL_C_HPP
#define OS1_SYSCALL_C_HPP

#include "../lib/hw.h"

void *mem_alloc(size_t size);
int mem_free(void*);

class TCB;
typedef TCB *thread_t;
int thread_create(
        thread_t *handle,
        void(*start_routine)(void*),
        void *arg
);

int thread_create_not_ready(
        thread_t *handle,
        void(*start_routine)(void*),
        void *arg
);

int thread_start(thread_t handle);
int thread_exit();
void thread_dispatch();

class Sem;
typedef Sem *sem_t;
int sem_open(
        sem_t *handle,
        unsigned init
);

int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

const int EOF = -1;
char getc();
void putc(char);

#endif //OS1_SYSCALL_C_HPP
