//
// Created by os on 1/4/23.
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"
#include "../h/sem.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    if (TCB::running->is_systhread()) {
        ms_sstatus(SSTATUS_SPP);
    } else {
        mc_sstatus(SSTATUS_SPP);
    }
    __asm__ volatile ("sret");
}

using Body = void(*)(void*);

void Riscv::handleSupervisorTrap()
{
    volatile uint64 scause = r_scause();

    if (scause == 0x0000000000000009UL || scause == 0x0000000000000008UL) {
        // interrupt: no; cause code: environment call from S-mode(9) or U-mode(8)
        volatile uint64 sepc = r_sepc() + 4;
        volatile uint64 sstatus = r_sstatus();

        uint64 opcode;
        char c;
        volatile uint64* args = nullptr;
        sem_t* sem_handle;
        sem_t semHandle;
        __asm__ volatile("mv %0, a0" : "=r"(opcode));

        if(opcode == 0x01)              // MEM_ALLOC
        {
            size_t size;
            __asm__ volatile ("mv %0, a1" : "=r"(size));
            MemoryAllocator::mem_alloc(size);
        } else if(opcode == 0x02)       // MEM_FREE
        {
            void* addr;
            __asm__ volatile("mv %0, a1" : "=r"(addr));
            MemoryAllocator::mem_free(addr);
        } else if(opcode == 0x11)       // THREAD_CREATE
        {
            __asm__ volatile ("mv %[reg], a1" : [reg] "=r" (args));

            thread_t *handle = (thread_t *) args[0];
            Body routine = (Body) args[1];
            void *arg = (void *) args[2];
            uint64 *stack = (uint64*) args[3];

            *handle = TCB::createThread(routine, arg, stack, true);
        } else if(opcode == 0x12)       // THREAD_EXIT
        {
            TCB::exit();
        } else if(opcode == 0x13)       // THREAD_DISPATCH
        {
            TCB::dispatch();
        } else if(opcode == 0x14)       // THREAD_CREATE_NOT_READY
        {
            __asm__ volatile ("mv %[reg], a1" : [reg] "=r" (args));

            thread_t *handle = (thread_t *) args[0];
            Body routine = (Body) args[1];
            void *arg = (void *) args[2];
            uint64 *stack = (uint64*) args[3];

            *handle = TCB::createThread(routine, arg, stack, false);
        } else if(opcode == 0x15)       // THREAD_START
        {
            thread_t th;
            __asm__ volatile("mv %0, a1" : "=r"(th));
            th->TCB::start();
        } else if(opcode == 0x21)       // SEM_OPEN
        {
            __asm__ volatile("mv %0, a1" : "=r"(args));
            sem_handle = (sem_t*) args[0];
            unsigned val = args[1];
            *sem_handle = Sem::createSem(val);
        } else if(opcode == 0x22)       // SEM_CLOSE
        {
            __asm__ volatile ("mv %0, a1" : "=r"(sem_handle));
            Sem::deleteSem(*sem_handle);
        } else if(opcode == 0x23)       // SEM_WAIT
        {
            __asm__ volatile ("mv %0, a1" : "=r"(semHandle));
            semHandle->wait();
        } else if(opcode == 0x24)       // SEM_SIGNAL
        {
            __asm__ volatile ("mv %0, a1" : "=r"(semHandle));
            semHandle->signal();
        } else if(opcode == 0x41)       // GETC
        {
            c = __getc();
            __asm__ volatile ("mv a0, %0" : : "r"(c));
        } else if(opcode == 0x42)       // PUTC
        {
            __asm__ volatile("mv %0, a1" : "=r"(c));
            __putc(c);
        }
        __asm__ volatile ("sd a0, 80(s0)");
        w_sstatus(sstatus);
        w_sepc(sepc);
    } else if (scause == 0x8000000000000001UL) {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
    } else if (scause == 0x8000000000000009UL) {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    } else {
        // unexpected trap cause

        uint64 scause = r_scause();
        printString("scause: ");
        printInt(scause, 16);
        printString("\n");

        uint64 stval = r_stval();
        printString("stval: ");
        printInt(stval, 16);
        printString("\n");

        uint64 stvec = r_stvec();
        printString("stvec: ");
        printInt(stvec, 16);
        printString("\n");

        uint64 sepc = r_sepc();
        printString("sepc: ");
        printInt(sepc, 16);
        printString("\n");
    }

}
