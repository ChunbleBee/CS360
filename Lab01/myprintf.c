#include <stdio.h>

typedef unsigned int u32;
char *ctable = "0123456789ABCDEF";

void prints(char * str) {
    int s_index = 0;
    while (str[s_index] != '\0') {
        putchar(str[s_index]);
        s_index++;
    }
}

int rpu(u32 x, int base)
{  
    char c;
    if (x){
       c = ctable[x % base];
       rpu(x / base, base);
       putchar(c);
    }
}

void printnum(u32 n, int base) {
    if (n < 0) {
        putchar('-');
        n *= -1;
    }
    (n == 0) ? putchar('0') : rpu(n, base);
}

int printu(u32 n)
{
    (n == 0) ? putchar('0') : rpu(n, 10);
}

/* printd() */
void printd(int n) {
    printnum(n, 10);
}

/* printx() */
void printx(u32 n) {
    printnum(n, 16);
}

/* printo() */
void printo(u32 n) {
    printnum(n, 8);
}

void myprintf(char * str, ...) {
    int * stkptr = (int *) getebp() + 3;
    int s_index = 0, a_index = 0;

    while (str[s_index] != '\0') {
        if (str[s_index] == '%') {
            int stkptr_add = 1, s_index_add = 2;
            switch (str[s_index + 1]){
                case 'c': {
                    putchar(* stkptr);
                } break;

                case 's': {
                    prints(* stkptr);
                } break;

                case 'u': {
                    printu(* stkptr);
                } break;
                
                case 'd': {
                    printd(* stkptr);
                } break;

                case 'o': {
                    printo(* stkptr);
                } break;

                case 'x': {
                    printx(* stkptr);
                } break;

                default: {
                    putchar('%');
                    stkptr_add = 0;
                    s_index_add = 1;
                } break;
            }
            stkptr += stkptr_add;
            s_index += s_index_add;
        } else {
            putchar(str[s_index]);
            if (str[s_index] == '\n') putchar('\r');
            s_index++;
        }
    }
    putchar('\n');
}

int main(int argc, char *argv[], char *env[]) {
    prints("Shobe doobee\ndoowop bang boom!\n");
    myprintf("cha=%c string=%s\
    dec=%d hex=%x oct=%o neg=%d\n", 
        'A',
        "this is a test",
        100,
        100,
        100,
        -100);
    myprintf("Number of CMD line args (argc): %u\n", argc);
    for(int i = 0; i < argc; i++) {
        myprintf("Cmd line arg %i = %s\n", argv[i]);
    }
    myprintf("\n");
    for(int i = 0; env[i] != NULL; i++){
        myprintf("Env. Var. #%u = %s", i, env[i]);
    }
}