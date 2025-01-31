/*********** A Multitasking System ************/
#include <stdlib.h>

#include "wait.c"

PROC proc[NPROC], *running, *freeList, *readyQueue;
PROC *sleepList;  // for sleep()/wakeup() 

int body()
{
  char command[64];
  printf("proc %d resume to body()\n", running->pid);
  while(1){
    printf("***************************************\n");
    printf("-- Process %d executing --\n\tParent Process: %d\n\n",
      running->pid,
      running->ppid);
    printf("input a command: [ps|fork|switch|exit|wait] : ");
    fgets(command, 64, stdin);
    command[strlen(command)-1] = 0;
    printf("\n");
    
    if (!strcmp(command, "ps"))
      do_ps();
    else if (!strcmp(command, "fork"))
      do_fork();
    else if (!strcmp(command, "switch"))
      do_switch();
    else if (!strcmp(command, "exit"))
      do_exit();
    else if (!strcmp(command, "wait"))
      do_wait();
    else
      printf("invalid command\n");
  }
}

int do_switch()
{
   printf("proc %d switch task\n", running->pid);
   tswitch();
   printf("proc %d resume\n", running->pid);
}

int do_fork()
{
   int child = kfork(body);
   if (child < 0)
      printf("kfork failed\n");
   else{
      printf("proc %d kforked a child = %d\n", running->pid, child); 
      printList("readyQueue", readyQueue);
   }
   return child;
}

char *pstatus[]={"FREE   ","READY  ","SLEEP  ","BLOCK  ","ZOMBIE", "RUNNING"};
int do_ps()
{
  int i; PROC *p;
  printf("pid   ppid    status\n");
  printf("--------------------\n");
  for (i=0; i<NPROC; i++) {
     p = &proc[i];
     printf(" %d      %d     ", p->pid, p->ppid);
     if (p==running)
       printf("%s\n", pstatus[5]);
     else
       printf("%s\n", pstatus[p->status]);
  }
}

int do_exit()
{
  int value;
  PROC *p;
  if (running->pid==1){
      printf("P1 never dies\n");
      return 0;
  }
  printf("proc %d in do_exit(), enter an exit value : ", running->pid);
  scanf("%d", &value);
  getchar();
  kexit(value);
}

int init()
{
  int i;
  for (i = 0; i < NPROC; i++){
    proc[i].pid = i; 
    proc[i].status = FREE;
    proc[i].priority = 0;
    proc[i].next = (PROC *)&proc[(i+1)];
  }
  proc[NPROC-1].next = 0;
 
  freeList = &proc[0];        
  readyQueue = 0;

  // create P0 as the initial running process
  running = dequeue(&freeList);
  running->status = READY;
  running->priority = 0;
  running->parent = running;
  printList("freeList", freeList);
  printf("init complete: P0 running\n"); 
}

/*************** main() ***************/
int main()
{
   printf("\nWelcome to 360 Multitasking System\n");
   init();
   printf("P0 fork P1\n");
   kfork(body);  

   while(1) {
     if (readyQueue) {
        printf("P0: switch task\n");
        tswitch();
     }
   }
}

/*********** scheduler *************/
int scheduler()
{ 
  printf("proc %d in scheduler()\n", running->pid);
  if (running->status == READY)
      enqueue(&readyQueue, running);
  printList("readyQueue", readyQueue);
  running = dequeue(&readyQueue);
  printf("next running = %d\n", running->pid);  
}


