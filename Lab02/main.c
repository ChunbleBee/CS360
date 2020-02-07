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
typedef enum _pipe_mode {
    NONE,
    WRITE,
    TWOWAY,
    READ
} PipeMode;

char * paths = NULL;
char * workingDirectory = NULL;
char * homeDirectory = NULL;
char ** pathTokens = NULL;
u_int32_t numPaths = 0;

int readFD[2];
int writeFD[2];
char pipeBuffer[BUFFERLEN];
PipeMode pMode = NONE;

//Prototypes:
void getInput();
void printTokenArray(char ** tokens);
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
    if (interpreter(buffer)) {
        free(buffer);
    }
}

bool interpreter(char * buffer) {
    printf("--> interpreter()\n");
    char * curCommand = strtok(buffer, "|");
    char * nextCommand = strtok(NULL, "|");
    if (strncmp(curCommand, "#", 1) == 0) {
        printf("%s\n", &(curCommand[1]) );
    } else if (strncmp(curCommand, "exit", 4) == 0) {
        printf("exiting...\n");
        exit(0);
    } else if (strncmp(curCommand, "cd", 2) == 0) {
        printf("#TODO: changin' directoriez...\n");
    } else {
        //Break by input symbols
        char * leftHandSide = strtok(curCommand, "<");
        char * rightHandSide = strtok(NULL, "<");
        //toss the rest of the file, as there can only ever be one input file.

        u_int32_t cmdAndArgsLen = 0; //This is here to make the thingy happy.
        char ** cmdAndArgs = stringToTokenArray(leftHandSide, " ", &cmdAndArgsLen);
        printf("LHS: %s, RHS: %s\n", leftHandSide, rightHandSide);

        if (rightHandSide != NULL) {
            char * filePath = findFile(strtok(rightHandSide, " "), F_OK);
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
                } else {
                    printf("Cant execute file: %s", cmdPath);
                }
            } else {
                printf("Command '%s' cannot be found!!!\n", cmdAndArgs[0]);
            }
            free(cmdPath);
        }
    }
    if (nextCommand != NULL) {
        interpreter(nextCommand);
    }
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

int executeImage(char * fileName, char *const argv[], char *const envp[]) {
    printf("--> executeImage()\n");

    int forked = fork();

    if (forked >= 0) {
        if (forked == 0) {
            int output = execve(fileName, argv, envp);
            exit(output);
        } else {
            printf("Process %u forked child %u, switching tasks.\n\n", getpid(), forked);
            wait(&forked);
            printf("\n\n");
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