#include <stdio.h>

int main()
{
    int h;
    FILE *batch;
    char ext[5]=".txt";
    char name[15]="hp5_cp_00";
    char c = 48;
    for(int i=1;i<=38;i++){
        c++;
        name[8] = c;
        if(c==58) {
            name[7]++;
            c=48;
        }
        for(h=0;h<4;h++){
            name[9+h]=ext[h];
        }
        name[9+h] = '\0';
        batch = fopen(name, "w");
    }
}
