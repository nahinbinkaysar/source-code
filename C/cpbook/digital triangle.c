//problem-29 digital triangle
//nahin bin kaysar
//http://cpbook.subeen.com/2013/01/digital-trivuz.html

#include <stdio.h>

int main()
{
    int t,T,i,j,D,H;
    scanf("%d", &T);
    for(t=0;t<T;t++){
        scanf("%d %d",&D,&H);
        printf("Case %d:\n",t+1);
        for(i=1;i<=H;i++){
            for(j=0;j<i;j++){
                printf("%d",D);
            }
            printf("\n");
        }
    }
    return 0;
}
