//
// Created by os on 1/27/23.
//

#include "../h/sem.hpp"

Sem* Sem::createSem(unsigned init) {
    return new Sem(init);
}

Sem::~Sem() {
    while (val < 0) {
        TCB* t = blockedThreads.removeFirst();
        Scheduler::put(t);
        val++;
    }
}

int Sem::wait() {
    val--;
    if(val < 0)
    {
        TCB* t = TCB::running;
        t->setStatus(TCB::WAITING);
        blockedThreads.addLast(t);
        thread_dispatch();
    }
    return 0;
}

int Sem::signal() {
    val++;
    if(val <= 0)
    {
        TCB* t = blockedThreads.removeFirst();
        t->setStatus(TCB::READY);
        Scheduler::put(t);
    }
    return 0;
}

void Sem::deleteSem(sem_t handle) {
    delete handle;
}
