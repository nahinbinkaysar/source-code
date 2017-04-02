//problem-20 word seller
//nahin bin kaysar
//http://cpbook.subeen.com/2012/12/shobdo-bepari.html

#include <stdio.h>
#include <string.h>

int cnum(char str[])
{
    int i,len,count;
    count=0;
    len=strlen(str);

    for(i=0;i<len;) {
        if(str[i] == ' ') {
            while(str[i] == ' ') {
                i++;
            }
            count++;
        }
        else{
            i++;
        }
    }
    return count+1;
}

int main()
{
    int T,count;
    char str[1000000];
    scanf("%d", &T);
    while(T--) {
        getchar();
        gets(str);
        count=cnum(str);
        printf("%d\n",count*420);
    }

    return 0;
}

