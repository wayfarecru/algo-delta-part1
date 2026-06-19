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

int threshold = 5000000;
bool pred(int i) {
	return (i < threshold);
}

struct PairInd {
	int n1;
	int n2;
};

PairInd my_part_ind(int n, int A[], int B[], int C[], bool (*p)(int)) {
	int n1 = 0; // index to B[]
	int n2 = 0; // index to C[]
	for (int i = 0; i < n; ++i) {
		if (p(A[i]) == true) {
			B[n1++] = A[i];
		} else {
			C[n2++] = A[i];
		}
	}
	PairInd ans; // answer
	ans.n1 = n1;
	ans.n2 = n2;
	return ans;
}

struct PairPtr {
	int* ptr1;
	int* ptr2;
};

PairPtr my_part_ptr(int n, int* aptr, int* bptr, int* cptr, bool (*p)(int)) {
	while (n--) {
		if (p(*aptr)) {
			*bptr++ = *aptr++;
		} else {
			*cptr++ = *aptr++;
		}
	}
	PairPtr ans; // answer
	ans.ptr1 = bptr;
	ans.ptr2 = cptr;
	return ans;
}

std::pair<std::vector<int>::iterator, std::vector<int>::iterator> my_part_vec(
    std::vector<int>::iterator ait, const std::vector<int>::iterator alast,
    std::vector<int>::iterator bit, std::vector<int>::iterator cit, bool (*p)(int)) {
	while (ait != alast) {
		if (p(*ait)) {
			*bit++ = *ait++;
		} else {
			*cit++ = *ait++;
		}
	}
	return make_pair(bit, cit);
}

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 8);
	std::vector<int> A(N);
	std::vector<int> B(N);
	std::vector<int> C(N);
	// set data to be reversely ordered 
	cout << "original data:" << endl;
	for (int i = 0; i < A.size(); ++i) {
		A[i] = A.size() - i;
	}
	cout << "\tA[" << A.size() << "] = ";
	print(A);
	// array partition, using index
	cout << "array partition with indices:" << endl;
	threshold = N / 2;
	cout << "\tthreshold = " << threshold << endl;
	PairInd ans = my_part_ind(N, A.data(), B.data(), C.data(), pred);
	cout << "\tB[" << ans.n1 << "] = ";
	print<int>(B.begin(), std::next(B.begin(), ans.n1));
	cout << endl;
	cout << "\tC[" << ans.n2 << "] = ";
	print<int>(C.begin(), std::next(C.begin(), ans.n2));
	cout << endl;
	// array partition, using pointer
	cout << "array partition with pointers:" << endl;
	std::fill(B.begin(), B.end(), 0);
	std::fill(C.begin(), C.end(), 0);
	threshold = N / 4;
	cout << "\tthreshold = " << threshold << endl;
	PairPtr ptrs = my_part_ptr(N, A.data(), B.data(), C.data(), pred);
	int num1 = (ptrs.ptr1 - B.data());
	cout << "\tB[" << num1 << "] = ";
	print<int>(B.begin(), std::next(B.begin(), num1));
	cout << endl;
	int num2 = (ptrs.ptr2 - C.data());
	cout << "\tC[" << num2 << "] = ";
	print<int>(C.begin(), std::next(C.begin(), num2));
	cout << endl;
	// vector partition, iterator version
	cout << "vector partition with iterators:" << endl;
	std::fill(B.begin(), B.end(), 0);
	std::fill(C.begin(), C.end(), 0);
	threshold = 3 * N / 4;
	cout << "\tthreshold = " << threshold << endl;
	auto itpair = my_part_vec(A.begin(), A.end(), B.begin(), C.begin(), pred);
	cout << "\tB[" << std::distance(B.begin(), itpair.first) << "] = ";
	print<int>(B.begin(), itpair.first);
	cout << endl;
	cout << "\tC[" << std::distance(C.begin(), itpair.second) << "] = ";
	print<int>(C.begin(), itpair.second);
	cout << endl;
	// done
	return 0;
}
