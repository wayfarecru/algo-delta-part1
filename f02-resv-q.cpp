#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <vector>
#include <queue> // for priority_queue
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
	const int nElemPrint = 4; // number of printed elements, divided by 2
	if (v.size() <= 2 * nElemPrint) {
		for (const T e : v) {
			cout << e << " ";
		}
	} else { // 0 1 2 3 . . . 996 997 998 999
		for (int i = 0; i < nElemPrint; ++i) {
			cout << v[i] << " ";
		}
		cout << ". . .";
		for (int i = v.size() - nElemPrint; i < v.size(); ++i) {
			cout << " " << v[i];
		}
	}
	cout << endl;
}

std::random_device rd; // random device
std::default_random_engine re(rd()); // random engine

template<typename T>
void setRand(std::vector<T>& v) {
	if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
		std::uniform_real_distribution<T> unif_dist(0.0F, 1.0F);
		for (auto& a : v) {
			a = unif_dist(re);
		}
	} else {
		std::uniform_int_distribution<T> unif_dist(0, 9999999);
		for (auto& a : v) {
			a = unif_dist(re);
		}
	}
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
	// prepare priority queue
	std::vector<int> A(N); // input to the PQ
	std::vector<int> B; // internal buffer for PQ
	B.reserve(N);
	std::priority_queue<int> PQ { std::less<int>(), B }; // use B as the internal buffer
	// random number generate
	setRand(A);
	cout << "org:\tA[" << A.size() << "] : ";
	print(A);
	// sort with PQ
	std::vector<int> C;
	C.reserve(N); // reserve internal buffer to N elements
	{
		steady_clock::time_point start = steady_clock::now();
		for (int i = 0; i < A.size(); ++i) {
			PQ.push( A[i] );
		}
		while (! PQ.empty()) { // pop off remaining ones
			C.push_back( PQ.top() );
			PQ.pop();
		}
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "pri-q:\t\tC[" << C.size() << "] = ";
	print(C);
	// std::sort_heap
	{
		steady_clock::time_point start = steady_clock::now();
		std::make_heap(A.begin(), A.end(), std::greater());
		std::sort_heap(A.begin(), A.end(), std::greater());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "sort_heap:\tA[" << A.size() << "] : ";
	print(A);
	// diff
	cout << "equal(A,C) = " << std::boolalpha << std::equal(A.begin(), A.end(), C.begin()) << endl;
	// done
	return 0;
}
