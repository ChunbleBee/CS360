#include <stdio.h>
int g;                              
main()                              
{                                   
    static int a,b,c, d[1000]; 
    a = 1; b = 2; 
    c = a + b;
    printf("c=%d\n", c);
}
