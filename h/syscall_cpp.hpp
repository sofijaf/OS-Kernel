//
// Created by os on 1/25/23.
//

#ifndef OS1_SYSCALL_CPP_HPP
#define OS1_SYSCALL_CPP_HPP

#include "syscall_c.hpp"

void *operator new(size_t);
void operator delete(void*);

class Thread {
public:
    Thread (void (*body)(void*), void *arg);
    virtual ~Thread() { };

    int start();

    static void dispatch();
    static int sleep(time_t);
    static void wrapper(void*);

protected:
    Thread();
    virtual void run() { }

private:
    thread_t myHandle;
};

class Semaphore {
public:
    Semaphore(unsigned init=1);
    virtual ~Semaphore();

    int wait();
    int signal();

private:
    sem_t myHandle;
};

class Console {
public:
    static char getc();
    static void putc(char);
};

#endif //OS1_SYSCALL_CPP_HPP
