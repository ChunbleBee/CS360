#include <stddef.h>

const unsigned int NUM_PROCS = 10;
const unsigned int STACK_SIZE = 1024;

typedef enum EXECUTION_STATUS{
    FREE,
    READY,
    SLEEP,
    BLOCK,
    ZOMBIE
} STATUS;

typedef struct process_node {
    ProcessNode * parent;       // Ptr to Parent process
    ProcessNode * next;         // Ptr to next process in queue
    ProcessNode * sibling;      // Ptr to next sibling process

    // Ptrs to child process linked list
    ProcessNode * childHead;
    ProcessNode * childTail;

    unsigned int pid;           // This process' ID
    unsigned int ppid;          // Parent process' ID
    unsigned int priority;      // Execution Priority
    int wakeEvent;              // Event to wake upon
    int exitStatus;             // Process exit status
    STATUS execStatus;          // Current execution status of the process

    int stack[STACK_SIZE];      // Process' available stack space
    int * stackPtr;             // Current process stack pointer
} ProcessNode;

void initializeNode(ProcessNode * node) {
    node->parent = NULL;
    node->next = NULL;
    node->sibling = NULL;
    node->childHead = NULL;
    node->childTail = NULL;

    node->pid = 0;
    node->ppid = 0;
    node->priority = 1;
    node->wakeEvent = -1;
    node->exitStatus = 0;
    node->execStatus = FREE;
    node->stackPtr = &(node->stack);
}