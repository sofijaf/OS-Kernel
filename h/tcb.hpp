//
// Created by os on 1/4/23.
//

#ifndef OS1_V7_TCB_HPP
#define OS1_V7_TCB_HPP

#include "../lib/hw.h"
#include "../h/scheduler.hpp"
#include "../h/riscv.hpp"

class TCB
{
public:
    enum Status
    {
        FINISHED,
        RUNNING,
        READY,
        CREATED,
        IDLE,
        WAITING
    };

    ~TCB() { delete[] stack; }

    bool isFinished() const
    {
        return status == FINISHED;
    }

    bool isWaiting() const
    {
        return status == WAITING;
    }

    Status getStatus() { return status; }
    void setStatus(Status status) { this->status = status; }

    bool is_systhread() const { return sys_thread; }

    int start();

    using Body = void (*)(void*);

    static void yield();

    static TCB *kernelThread();

    static TCB *idleThread();

    static TCB *running;
    static TCB *kernel;
    static TCB *idle;

    static TCB* createThread(Body body, void* arg, uint64* stack, bool ready);

private:
    TCB(Body body, void* arg, uint64* stack, bool ready) :
            body(body),
            arg(arg),
            stack(stack),
            context({body != nullptr ? (uint64) &threadWrapper : 0,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0}),    // &stack[DEFAULT_STACK_SIZE / sizeof (uint64)]
            sys_thread(false)
    {
        if(body != nullptr && ready)
        {
            setStatus(READY);
            Scheduler::put(this);
        } else
        {
            setStatus(CREATED);
        }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    bool sys_thread;
    Status status;

    friend class Riscv;

    static void threadWrapper();

    static void idleWrapper(void* arg);

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static int exit();
};

#endif //OS1_V7_TCB_HPP
