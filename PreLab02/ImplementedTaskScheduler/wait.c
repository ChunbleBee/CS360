#include <string.h>
#include "queue.c"

extern PROC proc[];
extern PROC *running;
extern PROC *readyQueue;
extern PROC *freeList;
extern PROC *sleepList;

/********* Required PRE-work ***********/
// 1. Modify kfork() to implement process tree as a BINARY tree
int kfork(int (*func))
{
  PROC *p;
  int  i;
  /*** get a proc from freeList for child proc: ***/
  p = dequeue(&freeList);
  if (!p){
     printf("no more proc\n");
     return(-1);
  }

  /* initialize the new proc and its stack */
  p->status = READY;
  p->priority = 1;         // for ALL PROCs except P0
  p->ppid = running->pid;
  p->parent = running;
  p->sibling = NULL;
  
  // Add the new child to linked list of children nodes.
  PROC * child = p->parent->child;
  // If the process already has some child processes
  if (child != NULL) {
    // Find the end of the list.
    while (child->sibling != NULL) {
      child = child->sibling;
    }
    // Set the new child as the new end of the list
    child->sibling = p;
  } else {
    /* if not, set the parent's first child
    to the new process */
    p->parent->child = p;
  }

  //                    -1   -2  -3  -4  -5  -6  -7  -8   -9
  // kstack contains: |retPC|eax|ebx|ecx|edx|ebp|esi|edi|eflag|
  for (i=1; i<10; i++)
    p->kstack[SSIZE - i] = 0;

  p->kstack[SSIZE-1] = (int)func;
  p->saved_sp = &(p->kstack[SSIZE - 9]); 

  enqueue(&readyQueue, p);

  return p->pid;
}
// 2. Implement ksleep() per the algorithm in 3.4.1
int ksleep(int event)
{
  running->event = event;
  running->status = SLEEP;
  enqueue(&sleepList, running);
  tswitch();
}

// 2. Implement kwakeup per the algorithm in 3.4.2
int kwakeup(int event)
{
  PROC * sleep = sleepList, * sleepPrev = NULL;

  while (sleep != NULL) {
    if (sleep->event == event) {
      dequeue(sleep);
      // #TODO: if just dequeueing fucks up
      // on mid-list processes
      // Fix that shit.
      sleep->status = READY;
      enqueue(&readyQueue, sleep);
    }
    sleep = sleep->next;
  }
}

// 4. Modify kexit() to give away children to P1:
int kexit(int value)
{
  /*
  1)  Erase process user-mode content (delete that stack!)
  2)  Dispose of child processes if any.
  3)  Record exitValue in PROC->exitCode
  4)  Become a Zombie, but don't free the proc.
  5)  Wake up parent and, if needed, also
        the init process of Process 01.
  */

  // #TODO: Fix memset() s.t. it doesn't cause a segfault.
  //memset(running->kstack,0, sizeof(running->kstack));
  PROC * child = proc[1].child;
  if (child != NULL) {
    while (child->sibling != NULL){
      child = child->sibling;
    }
    child->sibling = running->child;
    child = child->sibling;
    while (child != NULL) {
      child->ppid = 1;
      child = child->sibling;
    }
  } else {
    // Theorhetically, this should never happen,
    // but ehhhhhhhhhh
    proc[1].child = running->child;
  }

  running->exitCode = value;
  running->status = ZOMBIE;
  kwakeup(ZOMBIE);
  tswitch();
}

// 3. Implement kwait() per the algorithm in 3.5.3
int kwait(int *status)
{
  PROC * next = running->child, * prev = NULL;
  
  while (next != NULL) {
    if (next->status == ZOMBIE) {
      int pid = next->pid;

      next->ppid = 0;
      next->status = FREE;
      (*status) = next->exitCode;

      if (prev != NULL) {
        prev->sibling = next->sibling;
      } else {
        running->child = next->sibling;
      }

      enqueue(&freeList, next);
      return pid;
    }

    prev = next;
    next = next->sibling;
  }
  ksleep(ZOMBIE);
  return -1;
}

// 4. Add a "wait" command to let proc wait for ZOMBIE child
int do_wait() {
  printList("freeList", freeList);
  int status = 0;
  int pid = kwait(&status);
  printf("proc %d waited for a ZOMBIE child process %d with exit status %d\n",
    running->pid,
    pid,
    status);
}