#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define BUFFERLEN 4000
char * paths = NULL;
char * workingDirectory = NULL;
char * homeDirectory = NULL;
char ** pathTokens = NULL;

//Prototypes:
char * findFile(char * command);
void getInput();
char * getEnvironmentVariable(char *env[], char * prefix);
char ** stringToTokenArray(char * string, char * delim, unsigned int * tokens);
int executeImage(char * fileName, char *const argv[], char *const envp[]);
char ** interpreter(char * string, unsigned int * tokens);

int main (int argc, char *argv[], char *env[]) {
    unsigned int numPaths = 0;

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
    printf("\n------------ Taiya's Terribly Tacky Terminal ------------\n");
    while (true) {
        getInput(pathTokens);
    }
}

char * findFile(char * command) {
    char * location = NULL;
    u_int32_t commandLen = strlen(command), totalLen = 0;
    printf("\tlength: %u\n", commandLen);
    
    return location;
}

void getInput() {
    char buffer[BUFFERLEN];
    printf("%s>", workingDirectory);
    fgets(buffer, BUFFERLEN, stdin);
    printf("\tinput: %s", buffer);
    u_int32_t len = 0;
    char ** tokens = stringToTokenArray(buffer, " ", &len);
    char * file = findFile(strtok(tokens[0], " "));

    if (file != NULL) {
        printf("\tFile found at: %s\n\tAttempting execution...", file);
        executeImage(file, (tokens + 1), __environ);
    } else {
        printf("File not found...\n");
    }
    free(tokens);
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

char ** stringToTokenArray(char * string, char * delim, unsigned int * tokens) {
    unsigned length = strlen(string);

    for (int i = 0; i < length; i++) {
        if (string[i] == (*delim) ) {
            (*tokens)++;
        }
    }
    char ** tokenArray = malloc(sizeof(char *) * (*tokens));
    
    if (tokenArray != NULL) {
        int i = 0;
        char * token = strtok(string, delim);
        while (token != NULL) {
            tokenArray[i] = token;
            i++;
            token = strtok(NULL, delim);
        }
    }
    return tokenArray;
}

int executeImage(char * fileName, char *const argv[], char *const envp[]) {
    execve(fileName, argv, envp);
}