#include "ProcessNode.h"
#include "Queue.c"

ProcessNode process[NUM_PROCS];
ProcessNode * runningProcess;
ProcessNode * readiedProcesses;
ProcessNode * freeProcesses;
ProcessNode * sleepingProcesses;

int init() {
    for (int i = 0; i < NUM_PROCS; i++) {
        process[i].pid = i;
        process[i].execStatus = FREE;
        process[i].priority = 0;
        process[i].next = (ProcessNode *)&process[(i+1)];
    }
    process[NUM_PROCS - 1].next = NULL;

    freeProcesses = &process[0];
    readiedProcesses = NULL;

    //Dequeue & init PID0
    runningProcess = dequeue(&freeProcesses);
    runningProcess->execStatus = READY;
    runningProcess->priority = 0;
    runningProcess->parent = runningProcess;
    printQueue(&freeProcesses, "Free Processes");
    printf("Initialization complete - PID0 Running");
}

int scheduler() {
    printf("Process %u in Task Scheduler.", runningProcess->pid);

    if (runningProcess->execStatus == READY) {
        priorityEnqueue(&readiedProcesses, runningProcess);
    }

    printQueue(readiedProcesses, "Readied Processes");
    runningProcess = dequeue(&readiedProcesses);
    printf("Next running process: %u", runningProcess->pid);
}

int kernel_fork(int * func) {
    ProcessNode * newProcess = dequeue(& freeProcesses);
    if (newProcess == NULL) {
        printf("No more processes available for forking!!\n");
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

    process[1].childTail->sibling = runningProcess->childHead;
    process[1].childTail = runningProcess->childTail;
    ProcessNode * child = runningProcess->childHead;
    while (child != NULL) {
        child->parent = (&process[1]);
        child->ppid = process[1].pid;
    }
    runningProcess->exitStatus = exitValue;
    runningProcess->execStatus = ZOMBIE;

    kernel_wakeup(runningProcess->parent); //#TODO: Fixit, ya dixit.
}

int kernel_wait(int * status) {
    ProcessNode * process = runningProcess->childHead, * prev = NULL;
    if (process == NULL) {
        return -1;
    }

    while(process != NULL) {
        if (process->execStatus == ZOMBIE) {
            int zombiePID = process->pid;
            (* status) = process->exitStatus;

            if (prev != NULL) {
                prev->sibling = process->sibling;
            } else {
                runningProcess->childHead = process->next;
                if (process->sibling == NULL) {
                    runningProcess->childTail = NULL;
                }
            }

            initializeNode(process);
            process->pid = zombiePID;
            return zombiePID;
        }
        prev = process;
        process = process->sibling;
    }
    return -1;
}
