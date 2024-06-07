//
// Created by os on 1/4/23.
//

#ifndef OS1_V7_SCHEDULER_HPP
#define OS1_V7_SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler {
private:
    static List<TCB> readyThreadQueue;

public:
    static TCB *get();

    static void put(TCB *tcb);
};

#endif //OS1_V7_SCHEDULER_HPP
