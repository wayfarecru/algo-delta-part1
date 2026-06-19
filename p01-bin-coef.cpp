#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <cmath> // for gamma function
using namespace std;
using namespace std::chrono;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

std::pair<int, int> parse_num_2(int argc, char* argv[], int n = 8, int k = 4) {
	if (argc >= 3) { // get argv[1] and argv[2] as int
		char* ptr = NULL;
		n = static_cast<int>(strtol(argv[1], &ptr, 0));
		k = static_cast<int>(strtol(argv[2], &ptr, 0));
	}
	return make_pair(n, k);
}

// --------------------------------------------------------------------------------

template <typename T>
T bin_coef_rec(int n, int k) {
	if (n <= 1 || k <= 0 || k >= n) {
		return 1;
	}
	if (k > n / 2) {
		k = n - k; // nCk == nC(n-k)
	}
	return bin_coef_rec<T>(n - 1, k - 1) + bin_coef_rec<T>(n - 1, k);
}

template <typename T>
T bin_coef_map(int n, int k) {
	if (n <= 1 || k <= 0 || k >= n) {
		return 1;
	}
	if (k > n / 2) {
		k = n - k; // nCk == nC(n-k)
	}
	static std::map<std::pair<int, int>, T> bc;
	auto it = bc.find(make_pair(n, k));
	if (it != bc.end()) {
		return it->second; // returns value of the map with (key,value) elements
	}
	T ans = bin_coef_map<T>(n - 1, k - 1) + bin_coef_map<T>(n - 1, k);
	bc[make_pair(n, k)] = ans;
	return ans;
}

template <typename T>
T bin_coef_bottomup(int n, int k) {
	if (n <= 1 || k <= 0 || k >= n) {
		return 1;
	}
	if (k > n / 2) {
		k = n - k; // nCk == nC(n-k)
	}
	static std::vector<std::vector<T>> bc; // Pascal's triangle
	bc.resize(n + 1); // i ranges [0..n]
	for (int i = 0; i <= n; ++i) {
		bc[i].resize(i + 1); // j ranges in [0..i]
	}
	for (int i = 0; i <= n; ++i) {
		bc[i][0] = 1;
		for (int j = 1; j < i; ++j) {
			bc[i][j] = bc[i - 1][j - 1] + bc[i - 1][j];
		}
		bc[i][i] = 1;
	}
	return bc[n][k];
}

long double bin_coef_gamma(int n, int k) {
	if (n <= 1 || k <= 0 || k >= n) {
		return 1;
	}
	if (k > n / 2) {
		k = n - k; // nCk == nC(n-k)
	}
	return tgammal(n + 1) / tgammal(k + 1) / tgammal(n - k + 1);
}

template <typename T>
T bin_coef_simple(int n, int k) {
	if (n <= 1 || k <= 0 || k >= n) {
		return 1;
	}
	if (k > n / 2) {
		k = n - k; // nCk == nC(n-k)
	}
	T numer = 1; // numeratort
	T denom = 1; // denominator
	for ( ; k >= 1; --n, --k) {
		numer *= n;
		denom *= k;
	}
	return numer / denom;
}

template <typename T>
T fac(int n) {
	T ans = 1;
	for (int i = 1; i <= n; ++i) {
		ans *= i;
	}
	return ans;
}

template <typename T>
T bin_coef_fac(int n, int k) {
	if (n <= 1 || k <= 0 || k >= n) {
		return 1;
	}
	if (k > n / 2) {
		k = n - k; // nCk == nC(n-k)
	}
	return fac<T>(n) / fac<T>(k) / fac<T>(n - k);
}

int main(int argc, char* argv[]) {
	int n, k;
	std::tie(n, k) = parse_num_2(argc, argv, 8, 4); // default: get C(8,4)
	// calculate nCk
	cout << "recursive evaluation:\t";
	if (n <= 35) {
		{
			steady_clock::time_point start = steady_clock::now();
			double ans_d = bin_coef_rec<double>(n, k);
			long ans_l = bin_coef_rec<long>(n, k);
			cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
			cout << "\tC(" << n << "," << k << ") = " << ans_d << " or " << ans_l << endl;
		}
	} else {
		cout << "cancelled due to time limit" << endl;
	}
	cout << "dyn prog with map:\t";
	{
		steady_clock::time_point start = steady_clock::now();
		double ans_d = bin_coef_map<double>(n, k);
		long ans_l = bin_coef_map<long>(n, k);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "\tC(" << n << "," << k << ") = " << ans_d << " or " << ans_l << endl;
	}
	cout << "dyn prog, bottom up:\t";
	{
		steady_clock::time_point start = steady_clock::now();
		double ans_d = bin_coef_bottomup<double>(n, k);
		long ans_l = bin_coef_bottomup<long>(n, k);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "\tC(" << n << "," << k << ") = " << ans_d << " or " << ans_l << endl;
	}
	cout << "eval with gamma func:\t";
	{
		steady_clock::time_point start = steady_clock::now();
		double ans_d = bin_coef_gamma(n, k);
		long ans_l = static_cast<long>(ans_d);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "\tC(" << n << "," << k << ") = " << ans_d << " or " << ans_l << endl;
	}
	cout << "simplified equation:\t";
	{
		steady_clock::time_point start = steady_clock::now();
		double ans_d = bin_coef_simple<double>(n, k);
		long ans_l = bin_coef_simple<long>(n, k);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "\tC(" << n << "," << k << ") = " << ans_d << " or " << ans_l << endl;
	}
	cout << "factorial equation:\t";
	{
		steady_clock::time_point start = steady_clock::now();
		double ans_d = bin_coef_fac<double>(n, k);
		long ans_l = bin_coef_fac<long>(n, k);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "\tC(" << n << "," << k << ") = " << ans_d << " or " << ans_l << endl;
	}
	// done
	return 0;
}
