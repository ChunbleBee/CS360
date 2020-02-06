#include <stddef.h>
#include <stdio.h>

#include "ProcessNode.c"

typedef struct _queue {
    ProcessNode * head;
    ProcessNode * tail;
} Queue;

void enqueue(Queue * queue, ProcessNode * node) {
    if (queue->head == NULL && queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    node->next = NULL;
}

void priorityEnqueue(Queue * queue, ProcessNode * node) {
    if (queue != NULL && node != NULL){
        ProcessNode * current = queue->head, * previous = NULL;
        while (current != NULL && node->priority <= current->priority) {
            previous = current;
            current = current->next;
        }

        if (previous == NULL) {
            queue->head = node;
        } else {
            previous->next = node;
        }
        if (current == NULL) {
            queue->tail = node;
        }
        node->next = current;
    }
}

ProcessNode * dequeue(Queue * queue) {
    ProcessNode * out = queue->head;
    if (out != NULL) {
        queue->head = out->next;
    }
    return out;
}

void printQueue(Queue * queue, char * queueName) {
    printf("{ %s }:\n", queueName);
    ProcessNode * current = queue->head;
    while (current != NULL) {
        printf(
            "\t| PID: %u, PPID: %u, Priority: %u, Status: %u, Wake Event: %d, Exit Code: %i\n",
            current->pid,
            current->ppid,
            current->priority,
            current->execStatus,
            current->wakeEvent,
            current->exitStatus
        );

        current = current->next;
    }
}