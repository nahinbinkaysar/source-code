//problem-70 perfect num-1
//nahin bin kaysar
//http://cpbook.subeen.com/2016/02/blog-post_3.html

#include <stdio.h>

int main()
{
    int T, i, sum;
    long long N;
    scanf("%d", &T);
    while(T--){
        sum = 0;
        scanf("%lld",&N);
        for(i=1;i<=N/2;i++){
            if(N%i==0){
                sum = sum + i;
            }
        }
        if(sum==N) printf("YES, %d is a perfect number!\n",N);
        else printf("NO, %d is not a perfect number!\n",N);
    }

    return 0;
}
