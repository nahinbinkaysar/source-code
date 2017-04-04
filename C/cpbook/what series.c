//problem-33 what series
//nahin bin kaysar
//http://cpbook.subeen.com/2013/01/kon-dhara.html

#include <stdio.h>

int main()
{
    int  T, t;
    double n1, n2, n3;
    scanf("%d", &T);
    for(t=0;t<T;t++) {
        scanf("%lf %lf %lf", &n1, &n2, &n3);
        if(n3-n2==n2-n1 && n3/n2==n2/n1) printf("Case %d: Both\n",t+1);
        else if(n3-n2==n2-n1) printf("Case %d: Arithmetic Progression\n",t+1);
        else if(n3/n2==n2/n1) printf("Case %d: Geometric Progression\n",t+1);
        else printf("Case %d: None\n",t+1);
    }
    return 0;
}
