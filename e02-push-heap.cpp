#include <iostream>
#include <random>
#include <chrono>
#include <vector>
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

const int Parent(int i) { return (i - 1) / 2; }
const int Left(int i) { return 2 * i + 1; }
const int Right(int i) { return 2 * (i + 1); }

template<typename T, typename Compare = std::less<T>>
void heapify(std::vector<T>& A, int n, int i, const Compare& compare = Compare()) {
	while (i <= (n - 2) / 2) { // only for internal nodes
		int largest = i;
		if (int l = Left(i); (l < n) && compare(A[largest], A[l])) {
			largest = l;
		}
		if (int r = Right(i); (r < n) && compare(A[largest], A[r])) {
			largest = r;
		}
		if (largest != i) {
			std::swap(A[i], A[largest]);
			i = largest;
		} else {
			return;
		}
	}
}

template<typename T, typename Compare = std::less<T>>
void buildHeap(std::vector<T>& A, int n, const Compare& compare = Compare()) {
	for (int i = (n - 2)/ 2; i >= 0; --i) {
		heapify(A, n, i, compare);
	}
}

template<typename T, typename Compare = std::less<T>>
void pushHeap(std::vector<T>& A, int i, const Compare& compare = Compare()) {
	T val = A[i]; // newly inserted element
	int parent = Parent(i);
	while ((i > 0) && compare(A[parent], val)) { // parent is smaller than value
		A[i] = A[parent];
		i = parent;
		parent = Parent(i);
	}
	A[i] = val;
}

template<typename T, typename Compare = std::less<T>>
void makeHeap(std::vector<T>& A, const Compare& compare = Compare()) {
	for (int i = 1; i < A.size(); ++i) {
		pushHeap(A, i, compare);
	}
}

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 1000000);
	std::vector<int> A(N);
	std::vector<int> B(N);
	std::vector<int> C(N);
	setRand(A);
	std::copy(A.begin(), A.end(), B.begin());
	std::copy(A.begin(), A.end(), C.begin());
	// my build-heap
	{
		steady_clock::time_point start = steady_clock::now();
		buildHeap(A, A.size(), std::greater());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "buildHeap:\tA[" << A.size() << "] : ";
	print(A);
	cout << "is_heap(A) = " << std::boolalpha << std::is_heap(A.begin(), A.end(), std::greater()) << endl;
	// my new version
	{
		steady_clock::time_point start = steady_clock::now();
		makeHeap(B, std::greater());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "makeHeap:\tB[" << B.size() << "] : ";
	print(B);
	cout << "is_heap(B) = " << std::boolalpha << std::is_heap(B.begin(), B.end(), std::greater()) << endl;
	// std::make_heap
	{
		steady_clock::time_point start = steady_clock::now();
		std::make_heap(C.begin(), C.end(), std::greater());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "std::make_heap:\tC[" << C.size() << "] : ";
	print(C);
	cout << "is_heap(C) = " << std::boolalpha << std::is_heap(C.begin(), C.end(), std::greater()) << endl;
	// diff
	cout << "equal(A,B) = " << std::boolalpha << std::equal(A.begin(), A.end(), B.begin()) << endl;
	cout << "equal(A,C) = " << std::boolalpha << std::equal(A.begin(), A.end(), C.begin()) << endl;
	cout << "equal(B,C) = " << std::boolalpha << std::equal(B.begin(), B.end(), C.begin()) << endl;
	// done
	return 0;
}
