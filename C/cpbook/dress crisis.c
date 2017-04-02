//problem-21 dress crisis
//nahin bin kaysar
//http://cpbook.subeen.com/2012/12/jama-shonkot.html

#include <stdio.h>

int main()
{
    int T,n;
    scanf("%d", &T);
    while(T--){
        scanf("%d", &n);
        if(n%2==0) printf("red\n");
        else printf("blue\n");
    }

    return 0;
}
