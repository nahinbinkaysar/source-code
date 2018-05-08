#include <bits/stdc++.h>
using namespace std;

int main()
{
    int ara[100001], high, j;
    int i, t, T, n;
    cin >> T;
    for(t = 0; t < T; t++) {
        cin >> n;
        int cnt[102] = {0};
        for(i = 0; i < n; i++) {
            cin >> ara[i];
            cnt[ara[i]]++;
        }

        high = cnt[101];
        for(i = 100; i >= 0; i--) {
            if(cnt[i] > high) {
                high = cnt[i];
                j = i;
            }
        }
        printf("Case %d: %d %d\n",t+1, j, high);
    }

    return 0;
}
