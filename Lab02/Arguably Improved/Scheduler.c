#include "ProcessNode.h"
#include "Queue.c"

ProcessNode processes[NUM_PROCS];
ProcessNode * runningProcess;
ProcessNode * readiedProcesses;
ProcessNode * freeProcesses;
ProcessNode * sleepingProcesses;

int init() {

}

int scheduler() {

}

int kernel_fork(int * func) {
    ProcessNode * newProcess = dequeue(& freeProcesses);
    if (newProcess == NULL) {
        printf("No more processes available!!\n");
        return -1;
    }

    newProcess->execStatus = READY;
    newProcess->priority = 1;
    newProcess->parent = runningProcess;
    newProcess->ppid = runningProcess->pid;
    newProcess->sibling = NULL;

    ProcessNode * childList = runningProcess->childTail;
    if (childList != NULL) {
        childList->sibling = newProcess;
    } else {
        runningProcess->childHead = newProcess;
    }
    runningProcess->childTail = newProcess;

    for (int i = 0; i < STACK_SIZE; i++) {
        newProcess->stack[i] = 0;
    }
    newProcess->stack[STACK_SIZE - 1] = (int)func;
    newProcess->stackPtr = &(newProcess->stack[STACK_SIZE - 9]);

    priorityEnqueue(&readiedProcesses, newProcess);
    return newProcess->pid;
}

int kernel_sleep(int event) {
    runningProcess->wakeEvent = event;
    runningProcess->execStatus = SLEEP;
    priorityEnqueue(&sleepingProcesses, runningProcess);
    tswitch();
}

int kernel_wakeup(int event) {
    ProcessNode * sleeper = sleepingProcesses, * prevSleeper = NULL;
    while (sleeper != NULL) {
        if (sleeper->wakeEvent == event) {
            ProcessNode * toWake = sleeper;

            if (prevSleeper != NULL) {
                prevSleeper->next = toWake->next;
            } else {
                sleepingProcesses = sleepingProcesses->next;
            }
            sleeper = sleeper->next;
            
            toWake->execStatus = READY;
            toWake->next = NULL;
            priorityEnqueue(&readiedProcesses, toWake);
        } else {
            prevSleeper = sleeper;
            sleeper = sleeper->next;
        }
    }
}

int kernel_exit(int exitValue) {
    for (int i = 0; i < STACK_SIZE; i++) {
        runningProcess->stack[i] = 0;
    }

    processes[1].childTail->sibling = runningProcess->childHead;
    processes[1].childTail = runningProcess->childTail;
    ProcessNode * child = runningProcess->childHead;
    while (child != NULL) {
        child->parent = (&processes[1]);
        child->ppid = processes[1].pid;
    }
    runningProcess->exitStatus = exitValue;
    runningProcess->execStatus = ZOMBIE;

    kernel_wakeup(runningProcess->parent); //#TODO: Fixit, ya dixit.
}

int kernel_wait(int * status) {
    ProcessNode * process = runningProcess->childHead;
    if (process == NULL) {
        return -1;
    }
    while(process != NULL) {
        if (process->execStatus == ZOMBIE) {
            int zombiePID = process->pid;
            (* status) = process->exitStatus;

            initializeNode(process);
            process->pid = zombiePID;
            return zombiePID;
        }
    }
    return -1;
}