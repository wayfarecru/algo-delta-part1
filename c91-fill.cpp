#include <iostream>
#include <random>
#include <algorithm> // for stable_sort
#include <cstring>
using namespace std;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

template<typename T>
void print(const std::vector<T>& v) {
	if (v.size() <= 10) {
		for (int i = 0; i < v.size(); ++i) {
			cout << v[i] << " ";
		}
	} else { // 0 1 2 3 4 . . . 995 996 997 998 999
		for (int i = 0; i < 5; ++i) {
			cout << v[i] << " ";
		}
		cout << ". . . ";
		for (int i = v.size() - 5; i < v.size(); ++i) {
			cout << v[i] << " ";
		}
	}
	cout << endl;
}

void setRand(std::vector<int>& v, std::default_random_engine& re) {
	std::uniform_int_distribution<int> unif_dist(0, 999);
	for (int i = 0; i < v.size(); ++i) {
		v[i] = unif_dist(re);
	}
}

int main(int argc, char* argv[]) {
	int N = 1000000;
	std::random_device rd; // random device
	std::default_random_engine re(rd()); // random engine
	// initializer list
	std::vector<int> A(N, 0);
	cout << "init:\t\tA[" << A.size() << "] : ";
	print(A);
	// for loop
	setRand(A, re);
	for (size_t i = 0; i < A.size(); ++i) {
		A[i] = 0;
	}
	cout << "for-loop:\tA[" << A.size() << "] : ";
	print(A);
	// memset
	setRand(A, re);
	memset(A.data(), 0, sizeof(int) * A.size());
	cout << "memset:\t\tA[" << A.size() << "] : ";
	print(A);
	// fill
	setRand(A, re);
	std::fill(A.begin(), A.end(), 0);
	cout << "fill:\t\tA[" << A.size() << "] : ";
	print(A);
	// fill_n
	setRand(A, re);
	std::fill_n(A.begin(), A.size(), 0);
	cout << "fill_n:\t\tA[" << A.size() << "] : ";
	print(A);
	// done
	return 0;
}
