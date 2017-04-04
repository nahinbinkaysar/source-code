//problem-68 num count-1
//nahin bin kaysar
//http://cpbook.subeen.com/2016/02/blog-post_4.html

#include <stdio.h>
#include <string.h>
int main()
{
    int T, count, len, i;
    char str[10001];
    scanf("%d", &T);
    while(T--){
        count = 0;
        scanf(" %[^\n]",str);
        len = strlen(str);
        for(i=0;i<len;i++){
            if(str[i]==' ') count++;
        }
        printf("Count = %d\n",count+1);
    }

    return 0;
}
