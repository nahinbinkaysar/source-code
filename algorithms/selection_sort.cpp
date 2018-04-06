#include <iostream>
using namespace std;

void selection_sort(int ara[], int n)
{
    int i, j, index_min, temp;

    for(i = 0; i < n-1; i++) {

        index_min = i;
        for(j = i+1; j < n; j++) {
            if(ara[j] < ara[index_min]) {
                index_min = j;
            }
        }

        if(index_min != i) {
            temp = ara[i];
            ara[i] = ara[index_min];
            ara[index_min] = temp;
        }
    }
}

int main()
{
    int n, i;
    cin >> n;
    int ara[n];

    for(i = 0; i < n; i++){
        cin >> ara[i];
    }

    selection_sort(ara, n);

    for(i = 0; i < n; i++){
        cout << ara[i] << " ";
    }

    return 0;
}
