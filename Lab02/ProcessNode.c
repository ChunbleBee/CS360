#include <stdio.h>
#include <stdbool.h>

typedef struct process_node {
    //this process ID
    int pid;

    //Parent pointers
    ProcessNode * parent;

    // Children Linked List controller pointers
    ProcessNode * head;
    ProcessNode * tail;

    //Sibling Linked List pointers
    ProcessNode * prev;
    ProcessNode * next;
} ProcessNode;

/*********************/
/* Node Constructors */
/*********************/
ProcessNode * createNewNode() {
    ProcessNode * pNew = malloc(sizeof(ProcessNode));
    if (pNew != NULL) {
        pNew->pid = -1;
        pNew->head = NULL;
        pNew->tail = NULL;
        pNew->prev = NULL;
        pNew->next = NULL;
        pNew->parent = NULL;
    }
    return pNew;
}

ProcessNode * initNewNode(ProcessNode * parent, int pid) {
    ProcessNode * pNew = createNewNode();
    if (pNew != NULL) {
        if (parent != NULL) {
            setParent(pNew, parent);
        }

        setPID(pNew, pid);
    }

    return pNew;
}

/********************/
/* Node Destructors */
/********************/
// #TODO

/******************************/
/* Process ID Getters/Setters */
/******************************/
int getPID(ProcessNode * process) {
    if (process != NULL) {
        return process->pid;
    }
    return -1;
}

int setPID(ProcessNode * process, int pid) {
    if (process != NULL) {
        process->pid = pid;
        return 1;
    }
    return 0;
}

ProcessNode * getParent(ProcessNode * process) {
    if (process != NULL) {
        return process->parent;
    }
    return NULL;
}

bool pushChild(ProcessNode * parent, ProcessNode * child) {
    if (parent != NULL && child != NULL) {
        if (getHead(parent) == NULL) {
            setHead(parent, child);
        }

        if (parent->tail != NULL) {
            setSibling(parent->tail, child);
        }

        setTail(parent, child);

        return true;
    }
    return false;
}

/******************************/
/* Sibling Getter and Setters */
/******************************/
void setSibling(ProcessNode * left, ProcessNode * right) {
    if (left != NULL) {
        left->next = right;
    }
    if (right != NULL) {
        right->prev = left;
    }
}

ProcessNode * getPrevSibling(ProcessNode * process) {
    if (process != NULL) {
        return process->prev;
    }
    return NULL;
}

ProcessNode * getNextSibling(ProcessNode * process) {
    if (process != NULL) {
        return process->next;
    }
    return NULL;
}