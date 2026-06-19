#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <vector>
#include <unordered_set>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <cstring>
using namespace std;
using namespace std::chrono;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

int parse_num(int argc, char* argv[], int n) {
	if (argc >= 2) { // get the argv[1] to long int
		char* ptr = NULL;
		long nn = strtol(argv[1], &ptr, 0);
		if (nn > 0) {
			n = nn;
		}
	}
	return n;
}

// --------------------------------------------------------------------------------

int fib_rec(int n) {
	if (n <= 1) return n; // F_0 = 0, F_1 = 1
	assert(n >= 2);
	return fib_rec(n - 2) + fib_rec(n - 1);
}

int fib_vec(int n) {
	if (n <= 1) return n; // F_0 = 0, F_1 = 1
	std::vector<int> f(n + 1);
	f[0] = 0;
	f[1] = 1;
	for (int i = 2; i <= n; ++i) {
		f[i] = f[i - 2] + f[i - 1];
	}
	return f[n];
}

int fib_var(int n) {
	if (n <= 1) return n; // F_0 = 0, F_1 = 1
	int f_m_2 = 0;
	int f_m_1 = 1;
	int f;
	for (int i = 2; i <= n; ++i) {
		f = f_m_2 + f_m_1;
		f_m_2 = f_m_1;
		f_m_1 = f;
	}
	return f;
}

int fib_mem(int n) {
	if (n <= 1) return n; // F_0 = 0, F_1 = 1
	static std::vector<int> f;
	if (n + 1 < f.size()) {
		int old_size = f.size();
		f.resize(n + 1);
		for (int i = old_size; i < n + 1; ++i) {
			f[i] = -1; // marking as NOT CALCULATED
		}
	}
	if (f[n] == -1) {
		return fib_mem(n - 2) + fib_mem(n - 1);
	} else {
		return f[n];
	}
}

int main(int argc, char* argv[]) {
	int n = parse_num(argc, argv, 10);
	// calculate n-th Fibonacci number
	{
		steady_clock::time_point start = steady_clock::now();
		int ans = fib_rec(n);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "F(" << n << ") = " << ans << endl;
	}
	{
		steady_clock::time_point start = steady_clock::now();
		int ans = fib_vec(n);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "F(" << n << ") = " << ans << endl;
	}
	{
		steady_clock::time_point start = steady_clock::now();
		int ans = fib_var(n);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "F(" << n << ") = " << ans << endl;
	}
	{
		steady_clock::time_point start = steady_clock::now();
		int ans = fib_var(n);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "F(" << n << ") = " << ans << endl;
	}
	// done
	return 0;
}
