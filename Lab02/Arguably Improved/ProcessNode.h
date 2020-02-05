const int NUM_PROCS = 10;
const int STACK_SIZE = 1024;

typedef enum {
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
    int wakeEvent;              // Event to wake on
    int exitStatus;             // Process exit status
    STATUS execStatus;          // Current execution status of the process

    int stack[STACK_SIZE];      // Process' available stack space
    int * stackPtr;             // Current process stack pointer
} ProcessNode;