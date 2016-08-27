#include <stdio.h>

int main(){
    int A[3][5];
    printf("%p\n",A);
    printf("%p\n",*A);
    printf("%d\n",**A);
    return 0;
}