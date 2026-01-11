/*
ID: nahinbi1
TASK: friday
LANG: C++
*/
#include <bits/stdc++.h>
using namespace std;
#define fast_cin() ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL)
#define all(x) x.begin(),x.end()
#define ll long long
#define gap ' '
#define endl '\n'
#define vi vector<int>
#define debug cout << "DEBUG: " << 

void solve()
{
	ifstream f("friday.in");
	vi mo = {31,28,31,30,31,30,31,31,30,31,30,31};
	int n; f >> n;
	n = 1900 + n - 1;
	deque<int> week(7,0);
	int startDay = 3;
	for (int i = 1900; i <= n; i++) {
		// cout << i << "\t";
		for (int j = 1; j <= 12; j++) {
			// cout << startDay << gap;
			week[(12+startDay)%7]++;
			if((i%4==0 and (i%100!=0 or i%400==0)) and j==2) startDay = (startDay + 29) % 7;
			else startDay = (startDay + mo[j-1]) % 7;
		}
	}	
	week.push_back(week[0]);
	week.pop_front();
	ofstream ff("friday.out");
	for (int i = 0; i < week.size()-1; i++) {
		ff << week[i] << gap;
	}
	ff << week[week.size()-1] << endl;
}

int main()
{
	solve();
	return 0;
}