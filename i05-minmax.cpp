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

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 1000000);
	// random data
	cout << "random data ----------" << endl;
	std::vector<int> A(N);
	setRand(A);
	cout << "\tA[0.." << A.size() << ") = ";
	print(A);
	// min_element, max_element
	cout << "std::min_element, std::max_element: " << endl;
	std::vector<int>::iterator min_it;
	std::vector<int>::iterator max_it;
	{
		steady_clock::time_point start = steady_clock::now();
		min_it = std::min_element(A.begin(), A.end());
		max_it = std::max_element(A.begin(), A.end());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "\tmin: A[" << std::distance(A.begin(), min_it) << "] = " << *min_it << endl;
	cout << "\tmax: A[" << std::distance(A.begin(), max_it) << "] = " << *max_it << endl;
	// minmax_element
	cout << "std::minmax_element: " << endl;
	std::pair<std::vector<int>::iterator, std::vector<int>::iterator> pair;
	{
		steady_clock::time_point start = steady_clock::now();
		pair = std::minmax_element(A.begin(), A.end());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "\tmin: A[" << std::distance(A.begin(), pair.first) << "] = " << *pair.first << endl;
	cout << "\tmax: A[" << std::distance(A.begin(), pair.second) << "] = " << *pair.second << endl;
	// sort
	cout << "std::sort for all range: " << endl;
	{
		steady_clock::time_point start = steady_clock::now();
		std::sort(A.begin(), A.end());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "\tA[0] = " << A[0] << endl;
	cout << "\tA[N-1] = " << A[A.size() - 1] << endl;
	// done
	return 0;
}
