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
void my_merge(std::vector<T>& A, std::vector<T>& B, int p, int q, int r, Compare& compare = Compare()) {
	std::copy(&A[p], &A[r], &B[p]);
	// merge B[p..q) and B[q..r) to A[p..r)
	int i = p; // index for B[p..q)
	int j = q; // index for B[q..r)
	int k = p; // index to A[p..r)
	while ((i < q) && (j < r)) {
		if (compare(B[i], B[j])) {
			A[k++] = B[i++];
		} else {
			A[k++] = B[j++];
		}
	}
	std::copy(&B[i], &B[q], &A[k]);
	std::copy(&B[j], &B[r], &A[k]);
}

template<typename T, typename Compare = std::less<T>>
void my_merge_sort(std::vector<T>& A, std::vector<T>& B, int p, int r, Compare& compare = Compare()) {
	if ((r - p) <= 1) return; // if 0 or 1 element, just return
	int q = (p + r) / 2; // mid point
	my_merge_sort(A, B, p, q, compare); // sort A[p..q)
	my_merge_sort(A, B, q, r, compare); // sort A[q..r)
	my_merge(A, B, p, q, r, compare);
}

template<typename T>
struct My_Less {
	int count;
	bool operator()(const T& a, const T& b) {
		++count;
		return (a < b);
	}
};

int main(int argc, char* argv[]) {
	const int N = 5;
	std::vector<int> A { 1, 2, 3, 4, 5 }; // all permutations
	std::vector<int> B(N); // input and output
	std::vector<int> C(N); // internal buffer
	My_Less<int> my_less;
	// for all permutations
	sort(A.begin(), A.end());
	int max_count = 0;
	do {
		std::copy(A.begin(), A.end(), B.begin());
		my_less.count = 0;
		my_merge_sort(B, C, 0, B.size(), my_less);
		cout << my_less.count << ": " ;
		print<int>(A.begin(), A.end());
		cout << "-> ";
		print<int>(B.begin(), B.end());
		cout << endl;
		max_count = std::max(max_count, my_less.count);
	} while (std::next_permutation(A.begin(), A.end()));
	cout << "max num of comp = " << max_count << endl;
	// done
	return 0;
}
