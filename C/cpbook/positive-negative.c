//problem-2 positive-negative
//nahin bin kaysar
//http://cpbook.subeen.com/2012/11/positive-negative.html

#include <stdio.h>

int main()
{
    int T, n, cpos = 0, cneg = 0;
    scanf("%d", &T);
    while(T--) {
        scanf("%d", &n);
        if(n>0) cpos++;
        else if(n<0) cneg++;
    }
    printf("%d %d\n",cpos,cneg);

    return 0;
}
