//http://www.codah.club/tasks.php?show_task=5000000063

#include <stdio.h>

int main()
{
    int n1, n2, n3, i;

    scanf("%d",&n1);
    int aran1[n1];
    for(i=0;i<n1;i++){
        scanf("%d", &aran1[i]);
    }

    scanf("%d",&n2);
    int aran2[n2];
    for(i=0;i<n2;i++){
        scanf("%d", &aran2[i]);
    }

    scanf("%d",&n3);
    int aran3[n3];
    for(i=0;i<n3;i++){
        scanf("%d", &aran3[i]);
    }

    int x;
    int array[3]={n1,n2,n3};
    int swapped, temp;
    for(x=0;x<2;x++){

        swapped=0;
        if(array[i]>array[i+1]){
            temp = array[i];
            array[i] = array[i+1];
            array[i+1] = temp;
            swapped=1;
        }
        if(swapped==0) break;
    }

    int d,e,f;
    int count=0;
    for(d=0;d<array[2];d++){
        for(e=0;e<array[2];e++){
            for(f=0;f<array[2];f++){
                if(aran1[d] == aran2[e] == aran3[f]) count++;
            }
        }
    }
    printf("%d\n",count);

    return 0;
}
