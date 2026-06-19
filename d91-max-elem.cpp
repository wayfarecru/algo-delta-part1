#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

template<typename T>
void print(const std::vector<T>& v) {
	if (v.size() <= 10) {
		for (const T e : v) {
			cout << e << " ";
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
	std::uniform_int_distribution<int> unif_dist(0, 999999);
	for (int i = 0; i < v.size(); ++i) {
		v[i] = unif_dist(re);
	}
}

int main(int argc, char* argv[]) {
	int N = 1000000;
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
			N = val;
		}
	}
	// random numbers
	std::random_device rd; // random device
	std::default_random_engine re(rd()); // random engine
	std::vector<int> A(N);
	setRand(A, re);
	cout << "A[" << A.size() << "] :\t";
	print(A);
	// pure c
	int* largest = &A[0];
	for (int i = 1; i < A.size(); ++i) {
		if (*largest < A[i]) {
			largest = &A[i];
		}
	}
	int ind0 = (largest - &A[0]);
	cout << "for loop:\t\tA[" << ind0 << "] = " << *largest << endl;
	// std::max_element
	std::vector<int>::iterator it = std::max_element(A.begin(), A.end());
	int ind = std::distance(A.begin(), it); // you got the index to the vector
	cout << "std::max_element:\tA[" << ind << "] = " << *it << endl;
	// done
	return 0;
}
