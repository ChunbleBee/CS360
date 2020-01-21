/************* t.c file ********************/
#include <stdio.h>
#include <stdlib.h>

int *FP;

int main(int argc, char *argv[ ], char *env[ ])
{
    int a,b,c;
    printf("enter main\n");

    printf("&argc = %x, argv = %x, env = %x\n", &argc, argv, env);
    printf("&a = %8x, &b = %8x, &c = %8x\n", &a, &b, &c);

    //(1). Write C code to print values of argc and argv[] entries
    printf("argc = %i\n Cmd line args (argvs)...\n", argc);
    for(int i = 0; i < argc; i++){
        printf("\t%i = %s\n", i, argv[i]);
    }

    a=1; b=2; c=3;
    A(a,b);
    printf("exit main\n");
}

int A(int x, int y)
{
    int d,e,f;
    printf("enter A\n");
    // write C code to PRINT ADDRESS OF d, e, f
    printf("Addresses of...\n\td = %x\n\te = %x\n\tf = %x\n",&d, &e, &f);
    d=4; e=5; f=6;
    B(d,e);
    printf("exit A\n");
}

int B(int x, int y)
{
    int g,h,i;
    printf("enter B\n");
    // write C code to PRINT ADDRESS OF g,h,i
    printf("Addresses of...\n\tg = %x\n\th = %x\n\ti = %x\n",&g, &h, &i);
    g=7; h=8; i=9;
    C(g,h);
    printf("exit B\n");
}

int C(int x, int y)
{
    int u = 10, v = 11, w = 12, i = 13, * p;
    
    printf("enter C\n");

    // write C cdoe to PRINT ADDRESS OF u,v,w,i,p;
    printf("Addresses of...\n\tu = %x\n\tv = %x\n\tw = %x\n\ti = %x\n\t&p = %x\n\tp = %x\n", &u, &v, &w, &i, &p, p);
    //Added the derefed address of p and the add. it's pointing to. FOR SHOVELRY!! :D

    FP = (int *) getebp();  // FP = stack frame pointer of the C() function
    
    // (2). Write C code to print the stack frame link list.
    int * stkptr = FP;

    printf("Printing the call-stack addresses!!! :D\n");
    while (stkptr != 0) {
        printf(">> New Call Stack Frame <<\n\tCurrent: %x\n\tNext: %x\n", stkptr, *stkptr);
        stkptr = *stkptr;
    }
    printf("\n");

    p = (int *)&p;
    /* (3). Print the stack contents from p to the frame of main()
        YOU MAY JUST PRINT 128 entries of the stack contents.*/
    int n = 0;
    while (p != stkptr && p != 0 && n < 128) {
        printf("Address: %x --> \tInt Rep: %i\n", p, *p);
        printf("\t\t\tChar Rep: %c\n\t\t\tHex Rep: %x\n", *p, *p);
        p++;
        n++;
    }
    /* (4). On a hard copy of the print out, identify the stack contents
        as LOCAL VARIABLES, PARAMETERS, stack frame pointer of each function. */
}
