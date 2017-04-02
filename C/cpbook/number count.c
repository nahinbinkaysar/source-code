//problem-3 number count own creation
//nahin bin kaysar
//http://cpbook.subeen.com/2012/11/count-numbers.html

#include <stdio.h>
#include <string.h>

int main()
{
    int T, len, i, count;
    char str[10000];
    scanf("%d", &T);
    while(T--) {
        count=0;
        getchar();
        gets(str);
        len=strlen(str);
        for(i=0;i<len;i++) {
            if(str[i]>='0' && str[i]<='9' && str[i+1]==' ') {
                count++;
            }
        }
        printf("%d\n",count+1);
    }

    return 0;
}
