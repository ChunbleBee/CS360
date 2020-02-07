#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <regex.h>


#define BUFFERLEN 4000

char * paths = NULL;
char * workingDirectory = NULL;
char * homeDirectory = NULL;
char ** pathTokens = NULL;
u_int32_t numPaths = 0;

//Prototypes:
void getInput();
void printTokenArray(char ** tokens);
int executeImage(char * fileName, char *const argv[], char *const envp[], char * next);
bool parse(char * strings);
bool interpret(char * command);
char * findFile(char * command, int mode);
char * getEnvironmentVariable(char *env[], char * prefix);
char * makeNewPath(char * file, char * path);
char ** stringToTokenArray(char * string, char * delim, u_int32_t * tokens);

int main (int argc, char *argv[], char *env[]) {
    paths = getEnvironmentVariable(env, "PATH=");
    workingDirectory = getEnvironmentVariable(env, "PWD="); 
    homeDirectory = getEnvironmentVariable(env, "HOME=");
    pathTokens = stringToTokenArray(paths, ":", &numPaths);

    printf("Init Complete!\n");
    printf("Recognized paths:\n");
    printTokenArray(pathTokens);
    printf("\n");
    printf("Current home directory: %s\n", homeDirectory);
    printf("Current working directory: %s\n", workingDirectory);
    printf("\n<------------ Taiya's Terribly Tacky Terminal ------------>\n");
    while (true) {
        getInput();
    }
}

void getInput() {
    printf("--> getInput()\n");

    char * buffer = calloc(BUFFERLEN, sizeof(char));

    printf("%s> ", workingDirectory);
    fgets(buffer, BUFFERLEN, stdin);
    buffer[strlen(buffer) - 1] = '\0';
    if (interpret(buffer)) {
        free(buffer);
    }
}

bool interpret(char * buffer) {
    printf("--> interpret()\n");

    char * current = strtok(buffer, "|");
    char * next = strtok(NULL, "\0");

    if (strncmp(current, "#", 1) == 0) {
        printf("%s\n", &(current[1]) );
    } else if (strncmp(current, "exit", 4) == 0) {
        printf("exiting...\n");
        exit(0);
    } else if (strncmp(current, "cd", 2) == 0) {
        printf("#TODO: changin' directoriez...\n");
    } else {
        u_int32_t cmdAndArgsLen = 0; //This is here to make the thingy happy.
        char ** cmdAndArgs = stringToTokenArray(current, " ", &cmdAndArgsLen);
        char * cmdPath = findFile(cmdAndArgs[0], F_OK);

        if (cmdPath != NULL) {
            if (access(cmdPath, X_OK) == 0) {
                cmdAndArgs[0] = cmdPath;
                executeImage(cmdPath, cmdAndArgs, __environ, next);
            } else {
                printf("Cant execute file: %s", cmdPath);
            }
        } else {
            printf("Command '%s' cannot be found!!!\n", cmdAndArgs[0]);
        }

        free(cmdPath);
    }
    printf("\n");
    return true;
}

char * findFile(char * file, int mode) {
    printf("--> findFile()\n");
    u_int32_t triedPaths = 0;
    char * newPath = NULL;

    while (newPath == NULL && triedPaths < numPaths) {
        newPath = makeNewPath(file, pathTokens[triedPaths]);
        printf("Trying: %s\n", newPath);

        if (access(newPath, mode) == 0) {
            printf("Found!!\n");
        } else {
            triedPaths++;
            free(newPath);
            newPath = NULL;
        }
    }

    if (newPath == NULL) { //we've already found the function...
        newPath = makeNewPath(file, workingDirectory);
        printf("Trying: %s\n", newPath);
        if (newPath != NULL && access(newPath, F_OK) == 0) {
            printf("Found!!\n");
        } else {
            free(newPath);
            newPath = NULL;
        }
    }

    if (newPath == NULL) { //we've already found the function..
        newPath = makeNewPath(file, "");
        printf("Trying: %s\n", newPath);
        if (newPath != NULL && access(newPath, F_OK) == 0) {
            printf("Found!!\n");
        } else {
            free(newPath);
            newPath = NULL;
        }
    }

    return newPath;
}

char * makeNewPath(char * file, char * path) {
    printf("--> makeNewPath()\n");
    u_int32_t totalLen = 1 + strlen(file) + strlen(path);
    //Extra space for next /
    char * newPath = calloc(totalLen, sizeof(char));
    if (newPath != NULL) {
        strcat(newPath, path);
        strcat(newPath, "/");
        strcat(newPath, file);
    }
    return newPath;
}

char * getEnvironmentVariable(char *env[], char * prefix) {
    printf("--> getEnvironmentVariables()\n");
    int index = 0, prefixLength = strlen(prefix);
    while (
        env[index] != NULL &&
        strncmp(env[index], prefix, prefixLength) != 0
    ) {
        index++;
    }
    if (env[index] == NULL)
        return NULL;
    return (env[index] + prefixLength);
}

char ** stringToTokenArray(char * string, char * delim, u_int32_t * tokens) {
    printf("--> stringToTokenArray()\n");
    (*tokens) = 1; //Always returns, at least, the string itself.
    u_int32_t length = strlen(string);
    u_int32_t delimLen = strlen(delim);
    
    for (u_int32_t i = 0; i < length; i++) {
        for (u_int32_t j = 0; j < delimLen; j++) {
            if (string[i] == delim[j]) {
                (*tokens)++;
            }
        }
    }

    char ** tokenArray = calloc((*tokens) + 1, sizeof(char *));
    if (tokenArray != NULL) {
        tokenArray[0] = strtok(string, delim);
        u_int32_t i = 1;
        while (i < (*(tokens) + 1)) {
            tokenArray[i] = strtok(NULL, delim);
            i++;
        }
    }
    printTokenArray(tokenArray);
    return tokenArray;
}

int executeImage(char * fileName, char *const argv[], char *const envp[], char * next) {
    printf("--> executeImage()\n");
    int forked = fork();

    if (forked >= 0) {
        if (forked == 0) {
            if (next != NULL) {
                int pipeFD[2];
                printf("THERE WAS A PIPING FIGHT!!!\n");
                int pipez = pipe(pipeFD);
                if (pipez == 0) {
                    int forkz = fork();
                    if (forkz >= 0) {
                        if (forkz == 0) {
                            close(1);
                            dup(pipeFD[1]);
                            close(pipeFD[0]);
                        } else {
                            // parent
                            printf("Process %u forked child %u, switching tasks.\n\n", getpid(), forkz);
                            close(0);
                            dup(pipeFD[0]);
                            close(pipeFD[1]);
                            wait(&forkz);
                            interpret(next);
                            exit(0);
                        }
                    } else {
                        printf("Failed to fork process )= PID%u", getpid());
                    }
                } else {
                    printf("Failed to create pipe D=");
                }
            }
            char ** arg = argv;
            for (u_int32_t i = 0; arg[i] != NULL; i++) {
                if (strcmp(arg[i], "<") == 0) {
                    arg[i] = NULL;
                    int file = open(arg[i + 1], O_RDONLY, 0644);
                    close(0);
                    dup(file);
                } else if (strcmp(arg[i], ">") == 0) {
                    arg[i] = NULL;
                    int file = open(arg[i + 1], O_WRONLY | O_CREAT, 0644);
                    close(1);
                    dup(file);
                } else if (strcmp(arg[i], ">>") == 0) {
                    arg[i] = NULL;
                    int file = open(arg[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    close(1);
                    dup(file);
                }
            }

            int output = execve(fileName, argv, envp);
            exit(output);
        } else {
            printf("Process %u forked child %u, switching tasks.\n\n", getpid(), forked);
            printf("\n--------------------------------------------\n");
            wait(&forked);
            printf("\n--------------------------------------------\n");
        }
    } else {
        printf("Child fork failed D:\n");
    }
}

void printTokenArray(char ** tokens) {
    char * token = *tokens;
    for (u_int32_t i = 0; token != NULL; i++, token = tokens[i]) {
        printf("\t%s\n", token);
    }
}