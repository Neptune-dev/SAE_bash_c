#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[])
{
    int a = 5;
    a >>= 1;
    a |= 1;
    printf("%d\n", a);
    return 0;
}