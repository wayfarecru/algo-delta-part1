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
void my_qsort_tre(typename std::vector<T>::iterator first, typename std::vector<T>::iterator last,
                  const Compare& compare = Compare()) {
	int dist;
	while ((dist = std::distance(first, last)) > 1) {
		std::uniform_int_distribution<int> unif_dist(0, dist - 1); // on the closed interval [0..dist-1]
		int rand_walk = unif_dist(re);
		auto piv_val = *(std::next(first, rand_walk)); // pivot value
		auto mid1 = std::partition(first, last, [piv_val, compare](const T& x) {
			return compare(x, piv_val);
		});
		my_qsort_tre<T>(first, mid1, compare);
		auto mid2 = std::partition(mid1, last, [piv_val, compare](const T& x) {
			return ! compare(piv_val, x);
		});
		first = mid2;
	}
}

template<typename T, typename Compare = std::less<T>>
void my_test(std::vector<T>& A, const Compare& compare = Compare()) {
	std::vector<T> B(A.size());
	std::vector<T> C(A.size());
	// original data
	cout << "original data: " << endl;
	cout << "\tA[" << A.size() << "] = ";
	print(A);
	// my_qsort
	cout << "my_qsort_tre:";
	std::copy(A.begin(), A.end(), B.begin());
	{
		steady_clock::time_point start = steady_clock::now();
		my_qsort_tre<T>(B.begin(), B.end(), compare);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "\tB[" << B.size() << "] = ";
	print(B);
	// std::sort
	cout << "std::sort:";
	std::copy(A.begin(), A.end(), C.begin());
	{
		steady_clock::time_point start = steady_clock::now();
		std::sort(C.begin(), C.end(), compare);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "\tC[" << C.size() << "] : ";
	print(C);
	// diff
	cout << "equal(B,C) = " << std::boolalpha << std::equal(B.begin(), B.end(), C.begin()) << endl;
}

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 1000000);
	// random data
	cout << "random data ----------" << endl;
	std::vector<int> A(N);
	setRand(A);
	my_test(A);
	// random data, float, descending order
	cout << "random data, float, descending order ----------" << endl;
	std::vector<float> AA(N);
	setRand(AA);
	my_test(AA, std::greater());
	// reversely sorted
	cout << "reversely sorted ----------" << endl;
	for (int i = 0; i < A.size(); ++i) {
		A[i] = A.size() - i;
	}
	my_test(A);
	// all same value
	cout << "all same value ----------" << endl;
	for (auto& x : A) {
		x = 9;
	}
	my_test(A);
	// done
	return 0;
}
