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
int executeImage(char * fileName, char *const argv[], char *const envp[]);
bool interpreter(char * strings);
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
    for (int i = 0; i < numPaths; i++) {
        printf("\t%s\n", pathTokens[i]);
    }
    printf("\n");
    printf("Current home directory: %s\n", homeDirectory);
    printf("Current working directory: %s\n", workingDirectory);
    printf("\n<------------ Taiya's Terribly Tacky Terminal ------------>\n");
    while (true) {
        getInput();
    }
}

void getInput() {
    char * buffer = (char *)malloc(sizeof(char) * BUFFERLEN);
    printf("%s> ", workingDirectory);
    fgets(buffer, BUFFERLEN, stdin);
    buffer[strlen(buffer) - 1] = '\0';
    printf("getInput(), switch to interpreter()\n");
    if (interpreter(buffer)) {
        free(buffer);
    }
}

bool interpreter(char * buffer) {
    printf("interpreter()\n");
    u_int32_t length = 0;
    char ** bufferBrokenByPipes = stringToTokenArray(buffer, "|", &length);
    for(int i = 0; i < length; i++) {
        if (strncmp(bufferBrokenByPipes[i], "exit", 4) == 0) {
            printf("exiting...\n");
            exit(0);
        } else if (strncmp(bufferBrokenByPipes[i], "cd", 2) == 0) {
            printf("#TODO: changin' directoriez...\n");
        } else {
            //Now we need to break by input symbols
            char * leftHandSide = strtok(bufferBrokenByPipes[i], "<");
            char * rightHandSide = strtok(NULL, "<");

            u_int32_t cmdAndArgsLen = 0; //This is here to make the thingy happy.
            char ** cmdAndArgs = stringToTokenArray(leftHandSide, " ", &cmdAndArgsLen);

            //There can only ever be one input file, thus we throw away the 
            printf("LHS: %s, RHS: %s\n", leftHandSide, rightHandSide);
            if (rightHandSide != NULL) {
                char * filePath = findFile(rightHandSide, F_OK);
                if (filePath != NULL) {
                    if (access(filePath, O_RDONLY) == 0) {
                        printf("<-- #TODO --> : make pipe thing from input file to called process.");
                    } else {
                        printf("Can't open %s for reading!!!\n", filePath);
                    }
                } else {
                    printf("Can't find file %s!!!\n", rightHandSide);
                }
                free(filePath);
            } else {         
                char * cmdPath = findFile(cmdAndArgs[0], F_OK);
                if (cmdPath != NULL) {
                    if (access(cmdPath, X_OK) == 0) {
                        cmdAndArgs[0] = cmdPath;
                        executeImage(cmdPath, cmdAndArgs, __environ);
                    }
                } else {
                    printf("No such command: %s found!!!\n", cmdPath[0]);
                }
                free(cmdPath);
            }
        }
    }
    free(bufferBrokenByPipes);
    return true;
}

char * findFile(char * file, int mode) {
    u_int32_t triedPaths = 0;
    char * newPath = NULL;

    while (newPath == NULL && triedPaths < numPaths) {
        newPath = makeNewPath(file, pathTokens[triedPaths]);
        printf("New cmd path to try: %s\n", newPath);

        if (access(newPath, mode) == 0) {
            printf("Found!!\n");
        } else {
            triedPaths++;
            free(newPath);
            newPath = NULL;
        }
    }
    if (newPath == NULL) {
        newPath = makeNewPath(file, workingDirectory);
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
    u_int32_t totalLen = 1 + strlen(file) + strlen(path);
    //Extra space for next /
    char * newPath = (char *)malloc(sizeof(char) * totalLen);
    if (newPath != NULL) {
        strcat(newPath, path);
        strcat(newPath, "/");
        strcat(newPath, file);
    }
    return newPath;
}

char * getEnvironmentVariable(char *env[], char * prefix) {
    int index = 0, prefixLength = strlen(prefix);
    while (
        env[index] != NULL &&
        strncmp(env[index], prefix, prefixLength) != 0
    ) {
        index++;
    }
    return (env[index] + prefixLength);
}

char ** stringToTokenArray(char * string, char * delim, u_int32_t * tokens) {
    u_int32_t length = strlen(string);
    u_int32_t delimLen = strlen(delim);
    
    for (u_int32_t i = 0; i < length; i++) {
        for (int j = 0; j < delimLen; j++) {
            if (string[i] == delim[j]) {
                (*tokens)++;
            }
        }
    }

    if ((*tokens) == 0) { 
        (*tokens) = 1;//Return the full string.
    }
    char ** tokenArray = malloc(sizeof(char *) * (*tokens));
    if (tokenArray != NULL) {
        u_int32_t i = 0;
        char * token = strtok(string, delim);
        while (token != NULL) {
            *(tokenArray + i) = token;
            i++;
            token = strtok(NULL, delim);
        }
    }
    return tokenArray;
}

int executeImage(char * fileName, char *const argv[], char *const envp[]) {
    int forked = fork();

    if (forked >= 0) {
        if (forked == 0) {
            printf("Process %u forked child %u, switching tasks.\n\n", getpid(), forked);
            execve(fileName, argv, envp);
            printf("\n");
        } else {
            wait(&forked);
        }
    } else {
        printf("Child fork failed D:\n");
    }
}