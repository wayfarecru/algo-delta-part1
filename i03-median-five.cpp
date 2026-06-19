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
T my_median_five(typename std::vector<T>::iterator first, typename std::vector<T>::iterator last,
                 Compare compare = Compare()) {
	assert(std::distance(first, last) == 5);
	T a = *first;
	T b = *(first + 1);
	T c = *(first + 2);
	T d = *(first + 3);
	T e = *(first + 4);
	// core of the algorithm
	if (! compare(a, b)) std::swap(a, b);
	if (! compare(c, d)) std::swap(c, d);
	if (compare(a, c)) {
		a = e;
		if (! compare(a, b)) std::swap(a, b);
	} else {
		c = e;
		if (! compare(c, d)) std::swap(c, d);
	}
	if (compare(a, c)) {
		if (compare(b, c))	return b;
		else				return c;
	} else {
		if (compare(d, a))	return d;
		else				return a;
	}
}

int main(int argc, char* argv[]) {
	// const int N = 5;
	std::vector<int> A { 1, 2, 3, 4, 5 }; // all permutation also input
	// for all permutations
	sort(A.begin(), A.end());
	do {
		int median = my_median_five<int>(A.begin(), A.end());
		print<int>(A.begin(), A.end());
		cout << "-> median = " << median << endl;
		if (median != 3) {
			cout << "ERROR: median finding failure" << endl;
		}
	} while (std::next_permutation(A.begin(), A.end()));
	// done
	return 0;
}
