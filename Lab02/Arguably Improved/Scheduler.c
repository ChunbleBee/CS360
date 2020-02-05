#include "ProcessNode.h"
#include "Queue.c"

ProcessNode processes[];
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

    enqueue(&readiedProcesses, newProcess);
    return newProcess->pid;
}

int kernel_sleep() {

}

int kernel_exit() {

}

int kernel_wait() {

}