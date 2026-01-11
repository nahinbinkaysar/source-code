#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
#define fast_cin() ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL)
#define all(x) x.begin(),x.end()
#define ll long long
#define gap ' '
#define endl '\n'
#define vi vector<ll>
#define debug cout << "DEBUG: " << 
typedef tree<int,null_type,less<int>,rb_tree_tag,tree_order_statistics_node_update> ordered_set;


vi primeFactorization(ll x, vi &spf) {
	// O(logx)
	vi ans;
	while(x != 1) {
		ll prime = spf[x];
		ll cnt = 0;
		while(x%prime == 0) {
			cnt++;
			x = x/prime;
		}
		ans.push_back(prime);
	}
	return ans;
}

int main()
{
	fast_cin();
	// precomputation
	// sieve of Eratosthenes for spf
	// O(nlog(logn))
	ll maxN = 1e6;
	vector<bool> isPrime(maxN, true);
	isPrime[0] = false; // 0 not a prime
	isPrime[1] = false; // 1 not a prime
	vector<ll> spf(maxN, 1e9);
	for (ll i = 2; i < maxN; i++) {
		if(isPrime[i]) {
			spf[i] = i;
			for (ll j = i*i; j < maxN; j+=i) {
				isPrime[j] = false;
				spf[j] = min(spf[j], i);
			}
		}
	}

	vi occupied(1e5+1, 0);
	vi isActive(1e5+1, 0);

	ll n, m; cin >> n >> m;
	for (int _ = 0; _ < m; _++) {
		char c; cin >> c;
		ll i; cin >> i;
		if(c == '+') {
			if(isActive[i] == 1) {
				cout << "Already on\n";
				continue;
			}
			vi primeF = primeFactorization(i, spf);
			bool flag = 0;
			for(auto j:primeF) {
				if(occupied[j] != 0) {
					cout << "Conflict with " << occupied[j] << endl;
					flag = 1;
					break;
				}
			}
			if(flag) continue;
			for(auto j:primeF) {
				occupied[j] = i;
			}
			cout << "Success\n";
			isActive[i] = 1LL;
		}

		else {
			if(isActive[i] == 0) {
				cout << "Already off\n";
				continue;
			}
			vi primeF = primeFactorization(i, spf);
			for(auto j:primeF) {
				occupied[j] = 0;
			}
			cout << "Success\n";
			isActive[i] = 0LL;
		}

		
	}
	
	return 0;
}