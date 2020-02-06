#include <stdio.h>
#include <string.h>

#include "Scheduler.c"

enum bool { FALSE, TRUE }; //Because apparently this ain't a thing in C by default e_e
const char *PROCESS_STATUS[] = {"Free", "Ready", "Sleep", "Block", "Zombie", "Running"};

int body() {
    char command[64];
    printf("Process %d, Resumes in Body() function\n", runningProcess->pid);

    while(TRUE) {
        printf("<----------------------------->\n");
        printf("-- Process %u runningProcess --\n", runningProcess->pid);
        printf("\tParent Process: %u\n", runningProcess->ppid);
        printf("\nInput a command: [ ps | fork | switch | exit | wait ]: ");
        fgets(command, 64, stdin);
        command[strlen(command) - 1] = 0;
        printf("\n");

        if (strcmp(command, "ps") == 0) {
            do_ps();
        } else if (strcmp(command, "fork") == 0) {
            do_fork();
        } else if (strcmp(command, "switch") == 0) {
            do_switch();
        } else if (strcmp(command, "exit") == 0) {
            do_exit();
        } else if (strcmp(command, "wait") == 0) {
            do_wait();
        } else {
            printf("Invalid Command!!\n");
        }
    }
}

int do_switch() {
    printf("Process %u switching tasks...\n", runningProcess->pid);
    taskSwitch();
    printf("Process %u resuming!\n", runningProcess->pid);
}

int do_fork() {
    int child  = kernel_fork(body);
    if (child < 0) {
        printf("Process fork failed!!! D:\n");
    } else {
        printf(
            "Process %u forked child process %u\n",
            runningProcess->pid,
            child
        );
        printQueue(
            &readiedProcesses,
            "Readied Processes"
        );
    }
}

int do_ps() {
    printf("| PID          PPID          Status |\n");
    printf("|-----------------------------------|\n");
    for (int i = 0; i < NUM_PROCS; i++) {
        char * processStatus = (&process[i] == runningProcess)
            ? PROCESS_STATUS[i]
            : "Running";
        printf(
            "| %u%14u%16s |\n",
            process[i].pid,
            process[i].ppid,
            processStatus
        );
    }
}

int do_exit() {
    int exitValue;
    if (runningProcess->pid == 1) {
        printf("PID1 never dies, otherwise we get a black hole.\n");
        return 0;
    }
    printf(
        "Process %u exiting, enter exit value: ",
        runningProcess->pid
    );
    scanf("%d", &exitValue);
    getchar();//removes trailing \n
    kernel_exit(exitValue);
}

int do_wait() {

};

int main() {
    printf("Welcome to the CPTS360 Multitasking System\n");
    init();
    printf("PID0 forking to PID1\n");
    kernel_fork(body);
    
    while (TRUE) {
        if (readiedProcesses.head != NULL) {
            printf("PID0: Switch tasks!\n");
            taskSwitch();
        }
    }
}