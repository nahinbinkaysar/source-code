//problem-69 numcount-2
//nahin bin kaysar
//http://cpbook.subeen.com/2016/02/blog-post_78.html

#include <stdio.h>
#include <string.h>

int main()
{
    int i, len, count, T;
    char str[10000];

    scanf("%d",&T);
    while(T--){
        count = 0;
        getchar();
        gets(str);
        len = strlen(str);
        for(i=0;i<len;i++){
            if(str[i] == 39) str[i] = 'a';
            if(str[i] < 65) str[i]=' ';
            if(str[i] > 122) str[i]=' ';
            if(str[i] > 90 && str[i] < 97) str[i]=' ';

        }
        for(i=0;i<len;){
            if(str[i]==' '){
                while(str[i] == ' '){
                    i++;
                }
                count++;
            }

            else i++;
        }
        printf("Count = %d\n",count+1);
    }

    return 0;
}
