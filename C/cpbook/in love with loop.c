//problem-30 in love with loop
//nahin bin kaysar
//http://cpbook.subeen.com/2013/01/in-love-with-loop.html

#include <stdio.h>

int main()
{
    int a, b, c, x, y, z;
    int t, T, temp, swap, i, j, ara[3];

    scanf("%d", &T);

    for(t=0;t<T;t++){
        scanf("%d %d %d", &a, &b, &c);
        printf("Case %d:\n",t+1);
        for(x=0;x<=a;x++){
            for(y=0;y<=b;y++){
                for(z=0;z<=c;z++){
                    if((x!=y && y!=z && x!=z)&&(x<=y && y<=z)){
                        printf("%d %d %d\n",x,y,z);
                    }
                }
            }
        }
    }

    return 0;
}
