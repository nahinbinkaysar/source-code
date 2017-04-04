#include <stdio.h>

int maxNum(int ara[])
{
    int i, max;
    max = ara[0];
    for(i=0;i<3;i++) {
        if(ara[i]>max) max = ara[i];
    }

    return max;
}

int minNum(int ara[])
{
    int i, min;
    min = ara[0];
    for(i=0;i<3;i++) {
        if(ara[i]<min) min = ara[i];
    }

    return min;
}

int midNum(int ara[])
{
    int i, max, min, mid;

    min = minNum(ara);
    max = maxNum(ara);
    for(i=0;i<3;i++){
        if(ara[i] != max && ara[i] != min) {
            mid = ara[i];
        }
    }

    return mid;
}

int main()
{
    int ara[3];
    int A, B, C;
    int t, T;
    int a, i, j, k;
    scanf("%d", &T);
    for(t=0;t<T;t++){
        for(a=0;a<3;a++) {
            scanf("%d", &ara[a]);
        }
        A = minNum(ara);
        B = midNum(ara);
        C = maxNum(ara);
        printf("%d %d %d\n",A,B,C);
    }

    return 0;
}
