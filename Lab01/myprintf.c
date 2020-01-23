#include <stdio.h>

typedef unsigned int u32;
char *ctable = "0123456789ABCDEF";

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

void myprintf(char * str, ...){
    int s_index = 0, a_index = 0;

    while (str[s_index] != '\0') {
        if (str[s_index] == '%') {
            s_index++;
            a_index++;
            switch (str[s_index]){
                case 'c': {
                    
                }
            }
        } else {
            putchar(str[s_index]);
        }
    }

    putchar('\n');
}

void prints(char * str) {
    myprintf(str);
}

int main() {
    prints("Shobe doobee\ndoowop bang boom!");
}