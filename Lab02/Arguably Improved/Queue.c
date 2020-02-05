#include <stddef.h>
#include <stdio.h>

#include "ProcessNode.h"

void enqueue(ProcessNode ** queue, ProcessNode * node) {
    if (queue == NULL) {
        (*queue) = node;
    } else {
        while ((* queue)->next != NULL) {
            queue = (* queue)->next;
        }
        (*queue)->next = node;
    }
}

void priorityEnqueue(ProcessNode ** queue, ProcessNode * node) {
    if (queue == NULL) {
        (* queue) = node;
    } else {
        ProcessNode * q = (*queue);
        while (
            q->next != NULL ||
            node->priority < q->priority
        ) {
            q = q->next;
        }
        node->next = q->next;
        q->next = node->next;
    }
}

ProcessNode * dequeue(ProcessNode ** queue) {
    if ((*queue) != NULL) {
        ProcessNode * temp = (* queue);
        (* queue) = (* queue)->next;
        return temp;
    } else {
        return NULL;
    }
}

void printQueue(ProcessNode * q, char * queueName) {
    printf("{ %s }:\n", queueName);
    while (q != NULL) {
        printf(
            "\tPID: %u, PPID: %u, Priority: %u, Status: %u, Event: %d, Exit Code: %i]",
            q->pid,
            q->ppid,
            q->priority,
            q->execStatus,
            q->wakeEvent,
            q->exitStatus
        );

        q = q->next;
    }
}