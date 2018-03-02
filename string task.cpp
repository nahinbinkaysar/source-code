#include <bits/stdc++.h>
using namespace std;
int main()
{
    char str[110];
    cin >> str;
    int len = strlen(str);
    for(int k = 0; k < len; k++) {
        if(str[k] >= 'A' && str[k] <= 'Z') {
            str[k] = str[k] + 'a' - 'A';
        }
    }


    for(int k = 0; k < len; k++) {
        if(str[k] == 'a' || str[k] == 'e' || str[k] == 'i' || str[k] == 'o' || str[k] == 'u' || str[k] == 'y') {
            for(int h = k; str[h]!='\0' ; h++){
                str[h] = str[h+1];
            }
            k = 0;
        }
        //k = 0;
    }
    /*int len2 = strlen(str);
    for(int k=0;k<len2;k++){
        if(str[k] >= 'a' && str[k] <= 'z') {
            printf(".%c", str[k]);
        }
    }*/

    cout << str;
    //cout << endl;

    return 0;
}
