#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <vector>
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

template<typename T, typename Compare = std::less<T>>
void my_select(typename std::vector<T>::iterator first, typename std::vector<T>::iterator nth,
               typename std::vector<T>::iterator last, const Compare& compare = Compare()) {
	int dist = std::distance(first, last);
	if (dist <= 1) {
		return;
	}
	std::uniform_int_distribution<int> unif_dist(0, dist - 1); // on the closed interval [0..dist-1]
	int rand_walk = unif_dist(re);
	auto piv_val = *(std::next(first, rand_walk)); // pivot value
	auto mid1 = std::partition(first, last, [piv_val, compare](const T & x) {
		return compare(x, piv_val);
	});
	if (std::distance(first, nth) < std::distance(first, mid1)) {
		my_select<T>(first, nth, mid1, compare);
		return;
	}
	auto mid2 = std::partition(mid1, last, [piv_val, compare](const T & x) {
		return ! compare(piv_val, x);
	});
	if (std::distance(first, mid2) <= std::distance(first, nth)) {
		my_select<T>(mid2, nth, last, compare);
		return;
	}
}

template<typename T, typename Compare = std::less<T>>
void my_test(std::vector<T>& A, int k, const Compare& compare = Compare()) {
	std::vector<T> B(A.size());
	std::vector<T> C(A.size());
	// original data
	cout << "finding for k = " << k << endl;
	cout << "original data: " << endl;
	cout << "\tA[0.." << A.size() << ") = ";
	print(A);
	// my_select
	cout << "my_select:";
	std::copy(A.begin(), A.end(), B.begin());
	typename std::vector<T>::iterator bit = std::next(B.begin(), k);
	{
		steady_clock::time_point start = steady_clock::now();
		my_select<T>(B.begin(), bit, B.end(), compare);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "\tB[" << std::distance(B.begin(), bit) << "] = " << *bit << endl;
	// std::nth_element
	cout << "std::nth_element:";
	std::copy(A.begin(), A.end(), C.begin());
	typename std::vector<T>::iterator cit = std::next(C.begin(), k);
	{
		steady_clock::time_point start = steady_clock::now();
		std::nth_element(C.begin(), cit, C.end(), compare);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "\tC[" << std::distance(C.begin(), cit) << "] = " << *cit << endl;
	// sorted result
	cout << "after sorting: " << endl;
	std::sort(A.begin(), A.end(), compare);
	cout << "\tA[0.." << A.size() << ") = ";
	print(A);
	cout << "\tA[" << k << "] = " << A[k] << endl;
}

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 1000000);
	std::uniform_int_distribution<int> unif_dist(0, N - 1);
	// random data
	cout << "random data ----------" << endl;
	std::vector<int> A(N);
	setRand(A);
	int k = unif_dist(re);
	my_test(A, k);
	// random data, float, descending order
	cout << "random data, float, descending order ----------" << endl;
	std::vector<float> AA(N);
	setRand(AA);
	int kk = unif_dist(re);
	my_test(AA, kk, std::greater());
	// random data again for 2nd smallest
	cout << "random data ----------" << endl;
	setRand(A);
	my_test(A, 1);
	// random data again for median
	cout << "random data, float, descending order ----------" << endl;
	setRand(AA);
	my_test(AA, N / 2, std::greater());
	// done
	return 0;
}
