//
// Created by os on 1/25/23.
//

#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"

Thread::Thread(void (*body)(void*), void *arg)
{
    thread_create_not_ready(&myHandle, body, arg);
}

Thread::Thread() {
    thread_create_not_ready(&myHandle, Thread::wrapper, this);
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::wrapper(void *t)
{
    ((Thread*)t)->run();
}

int Thread::start() {
    thread_start(myHandle);
    return 0;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char ch) {
    ::putc(ch);
}