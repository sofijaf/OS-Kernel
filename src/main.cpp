//
// Created by os on 1/4/23.
//

#include "../h/tcb.hpp"
#include "../h/syscall_c.hpp"
#include "../test/printing.hpp"

extern void userMain();

void user_wrapper(void* arg)
{
    userMain();
}

int main()
{
    MemoryAllocator::init_memory();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    TCB *kernel = TCB::kernelThread();
    TCB *idle = TCB::idleThread();

    thread_t user;
    thread_create(&user, user_wrapper, nullptr);

    while (!user->isFinished()) {
        thread_dispatch();
    }

    delete kernel;
    delete idle;
    delete user;

    return 0;
}