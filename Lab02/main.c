#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

//strlen() was causing unknown segfaults. So fuck it.
unsigned int stringLength(char * str) {
    unsigned int length = 0;
    if (str != NULL) {
        while(str[length] != '\0') {
            length++;
        }
    }
    return length;
}

char * getEnvironmentPaths(char *env[]) {
    int index = 0;
    while (
        env[index] != NULL &&
        strncmp(env[index], "PATH=", 5) != 0
    ) {
        index++;
    }
    return env[index];
}

char ** tokenizeEnvironmentPaths(char * paths, unsigned int * numPaths) {
    printf("init\n");
    //strlen() was c
    unsigned int tokens = 0, length = stringLength(paths);

    for (int i = 0; i < length; i++) {
        if (paths[i] == ':') {
            tokens++;
        }
    }
    printf("past it.\n\tTokens: %u\n\tLength: %u", tokens, length);

    char ** pathTokens;
    *pathTokens = malloc(sizeof(char*) * tokens);
    
    if (*pathTokens != NULL) {
        int i = 0;
        char * path = strtok(paths, ':');
        while (path != NULL) {
            printf("\t%s\n", path);
            pathTokens[i] = path;
            i++;
            path = strtok(paths, ':');
        }
    }
    return pathTokens;
}

char * getInput() {}

int executeImage(char * fileName, char * paths, char *const argv[], char *const envp[]) {

}

int main (int argc, char *argv[], char *env[]) {
    char * paths = (getEnvironmentPaths(env) + 5); //+5 = Remove "PATH="
    unsigned int numPaths = 0;
    char ** pathTokens = tokenizeEnvironmentPaths(paths, &numPaths);

    printf("------------ Taiya's Terribly Tacky Terminal ------------\n\n");
    printf("Recognized Path variables:\n");
    for (int i = 0; i < numPaths; i++) {
        printf("\t%s", pathTokens[i]);
    }
}