#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <vector>
#include <list>
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

template<typename T>
void print(typename std::vector<T>::const_iterator first, typename std::vector<T>::const_iterator last) {
	const int nElemPrint = 4; // number of printed elements, divided by 2
	int n = std::distance(first, last);
	if (n <= 2 * nElemPrint) {
		std::copy(first, last, std::ostream_iterator<T>(std::cout, " "));
	} else {  // 0 1 2 3 . . . 996 997 998 999
		std::copy(first, std::next(first, 4), std::ostream_iterator<T>(std::cout, " "));
		cout << ". . . ";
		std::copy(std::prev(last, 4), last, std::ostream_iterator<T>(std::cout, " "));
	}
}

template<typename T>
void print(const std::vector<T>& v) {
	print<T>(v.begin(), v.end());
	cout << endl;
}

std::random_device rd; // random device
std::default_random_engine re(rd()); // random engine

template<typename T>
void setRand(typename std::vector<T>::iterator first, typename std::vector<T>::iterator last) {
	if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
		std::uniform_real_distribution<T> unif_dist(0.0F, 1.0F);
		for (typename std::vector<T>::iterator it = first; it != last; ++it) {
			*it = unif_dist(re);
		}
	} else {
		std::uniform_int_distribution<T> unif_dist(0, 9999999);
		for (typename std::vector<T>::iterator it = first; it != last; ++it) {
			*it = unif_dist(re);
		}
	}
}

template<typename T>
void setRand(std::vector<T>& v) {
	setRand<T>(v.begin(), v.end());
}

int parseNum(int argc, char* argv[], int num = 1000000) {
	if (argc >= 2) { // get the argv[1] to long int
		char* ptr = NULL;
		long val = strtol(argv[1], &ptr, 0);
		if (ptr != NULL && *ptr != '\0') {
			switch (*ptr) {
			case 'k': case 'K': val *= 1024; break; // kilo
			case 'm': case 'M': val *= (1024 * 1024); break; // mega
			case 'g': case 'G': val *= (1024 * 1024 * 1024); break; // giga
			}
		}
		if (val > 0) {
			return val;
		}
	}
	return num;
}

// --------------------------------------------------------------------------------

void my_bucket_sort(const std::vector<float>& A, std::vector<float>& C, float L, int K) {
	std::vector<std::list<float>> B(K);
	for (int i = 0; i < B.size(); ++i) {
		B[i].clear();
	}
	for (const float& x : A) {
		int ind = static_cast<int>(x * L);
		B[ind].push_back(x);
	}
	size_t max_size = 0;
	size_t min_size = A.size();
	for (int i = 0; i < B.size(); ++i) {
		if (! B[i].empty()) {
			max_size = std::max(max_size, B[i].size());
			min_size = std::min(min_size, B[i].size());
			B[i].sort(); // O(m log m)
		} else {
#if 0
			cout << "\tB[" << i << "] is empty" << endl;
#endif
		}
	}
	cout << "\tbucket size: min = " << min_size << ", max = " << max_size << endl;
	C.clear();
	for (int i = 0; i < B.size(); ++i) {
		for (float& x : B[i]) {
			C.push_back(x);
		}
	}
}

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 1000000);
	std::vector<float> A(N); // random data, input
	std::vector<float> C(N); // output
	// random data
	float L = static_cast<float>(N) / 5.0F; // each bucket will have 5 elements
	int K = static_cast<int>(L) + 1;
	cout << "N = " << N << ", K = " << K << ", L = " << L << endl;
	cout << "random data ------" << endl;
	setRand(A);
	cout << "\tA[" << A.size() << "] = ";
	print(A);
	// my bucket sort
	cout << "bucket sort ------" << endl;
	{
		steady_clock::time_point start = steady_clock::now();
		my_bucket_sort(A, C, L, K);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "\tC[" << C.size() << "] = ";
	print(C);
	// std::sort
	cout << "std::sort:" << endl;
	{
		steady_clock::time_point start = steady_clock::now();
		std::sort(A.begin(), A.end());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "\tA[" << A.size() << "] : ";
	print(C);
	// diff
	cout << "equal(A,C) = " << std::boolalpha << std::equal(A.begin(), A.end(), C.begin()) << endl;
	// done
	return 0;
}
