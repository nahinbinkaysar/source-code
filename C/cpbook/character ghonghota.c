#include <stdio.h>
#include <string.h>

int main()
{
    char str[1000000], c;
    int T, i, count, len;
    scanf("%d", &T);
    while(T--){
        count = 0;
        getchar();
        gets(str);
        scanf("%c", &c);
        len = strlen(str);
        for(i=0;i<len;i++){
            if(str[i]==c){
                count++;
            }
        }
        if(count>0) printf("Occurrence of '%c' in '%s' = %d\n", c, str, count);
        else if(count==0) printf("'%c' is not present\n",c);
    }

    return 0;
}
