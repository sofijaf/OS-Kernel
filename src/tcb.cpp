//
// Created by os on 1/4/23.
//

#include "../h/tcb.hpp"
#include "../h/syscall_c.hpp"

TCB* TCB::running = nullptr;
TCB *TCB::kernel = nullptr;
TCB *TCB::idle = nullptr;

void TCB::idleWrapper(void* arg)
{
    while (true) { thread_dispatch(); }
}

TCB* TCB::createThread(Body body, void* arg, uint64* stack, bool ready)
{
    return new TCB(body, arg, stack, ready);
}

TCB *TCB::idleThread()
{
    if (!idle) {
        uint64 *stack = (uint64*) mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
        idle = new TCB(idleWrapper, nullptr, stack, false);
        idle->status = IDLE;
        idle->sys_thread = true;
    }
    return idle;
}

TCB *TCB::kernelThread()
{
    if (!kernel) {
        TCB *thr = new TCB(nullptr, nullptr, nullptr, false);
        running = kernel = thr;
        kernel->sys_thread = true;
    }
    return kernel;
}

void TCB::yield() {
    Riscv::pushRegisters();
    TCB::dispatch();
    Riscv::popRegisters();
}

int TCB::exit()
{
    running->status = FINISHED;
    TCB::dispatch();
    return 0;
}

int TCB::start()
{
    if (status != CREATED) {
        return -1;
    }
    status = READY;
    Scheduler::put(this);
    return 0;
}

void TCB::dispatch() {
    TCB *old = running;
    if (!old->isFinished() && !old->isWaiting()) { Scheduler::put(old); }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    if(running->status != FINISHED)
    {
        thread_exit();
    }
}
