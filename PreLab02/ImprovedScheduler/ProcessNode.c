#include <stddef.h>

enum { STACK_SIZE = 1024 };

typedef enum {
    FREE,
    READY,
    SLEEP,
    BLOCK,
    ZOMBIE
} STATUS;

typedef struct _process_node {
    unsigned int pid;                   // This process' ID
    unsigned int * stackPtr;            // Current process stack pointer. TaskSwitch() breaks if moved.
    unsigned int ppid;                  // Parent process' ID
    unsigned int priority;              // Execution Priority

    int wakeEvent;                      // Event to wake upon
    int exitStatus;                     // Process exit status

    STATUS execStatus;                  // Current execution status of the process

    struct _process_node * next;        // Ptr to next process in queue
    struct _process_node * parent;      // Ptr to Parent process
    struct _process_node * sibling;     // Ptr to next sibling process
    struct _process_node * childHead;   // Ptr to child processes linked list head
    struct _process_node * childTail;   // Ptr to child processes linked list tail
    int stack[STACK_SIZE];              // Process' available stack space
} ProcessNode;

void initializeNode(ProcessNode * node, unsigned int pid) {
    node->parent = NULL;
    node->next = NULL;
    node->sibling = NULL;
    node->childHead = NULL;
    node->childTail = NULL;
    node->pid = pid;
    node->ppid = 0;
    node->priority = 0;
    node->wakeEvent = 0;
    node->exitStatus = 0;
    node->execStatus = FREE;
    node->stackPtr = &(node->stack);
}