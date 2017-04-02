//problem-23 magic
//nahin bin kaysar
//http://cpbook.subeen.com/2012/12/magic.html

#include <stdio.h>
#include <string.h>

int main()
{
    int i, T, ball, len;
    char str[510];
    scanf("%d",&T);
    while(T--) {
        scanf("%s",str);
        len=strlen(str);
        ball=0;
        for(i=0;i<len;i++) {
            if(str[i]=='0'){
                ball=ball-1;
            }
            if(str[i]=='1'){
                ball=ball+1;
            }
            if(ball<0){
                break;
            }
        }
        if(ball<0) printf("MAGIC\n");
        if(ball>=0) printf("NORMAL\n");
    }

    return 0;
}
