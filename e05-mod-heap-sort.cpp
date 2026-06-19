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

template<typename T, typename Compare = std::less<T>>
void heapify(std::vector<T>& A, int n, int i, const Compare& compare = Compare()) {
	while (i <= (n - 2) / 2) { // only for internal nodes
		int largest = i;
		int child = 2 * (i + 1); // right child
		if ((child < n) && compare(A[largest], A[child])) {
			largest = child;
		}
		child--; // left child
		if (compare(A[largest], A[child])) {
			largest = child;
		}
		if (largest == i) return;
		std::swap(A[i], A[largest]);
		i = largest;
	}
}

template<typename T, typename Compare = std::less<T>>
void buildHeap(std::vector<T>& A, int n, const Compare& compare = Compare()) {
	for (int i = (n - 2) / 2; i >= 0; --i) {
		heapify(A, n, i, compare);
	}
}

template<typename T, typename Compare = std::less<T>>
void adjustHeap(std::vector<T>& A, int hole, const int n, const T val, const Compare& compare = Compare()) {
	const int bound = (n - 1) / 2;
	int child = hole;
	while (child < bound) {
		child = 2 * child + 1; // left child
		if (compare(A[child], A[child + 1])) {
			child++; // now, right child is larger
		}
		if (compare(A[child], val)) { // val is the largest
			A[hole] = val;
			return;
		}
		A[hole] = A[child];
		hole = child;
	}
	if ((child == bound) && (n & 1) == 0 && compare(val, A[n - 1])) { // last left dangling child
		A[hole] = A[n - 1];
		hole = n - 1;
	}
	A[hole] = val;
}

template<typename T, typename Compare = std::less<T>>
void sortHeap(std::vector<T>& A, const Compare& compare = Compare()) {
	for (int i = A.size() - 1; i > 0; --i) {
		T val = A[i];
		A[i] = A[0];
		adjustHeap(A, 0, i, val, compare);
	}
}

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 1000000);
	std::vector<int> A(N);
	std::vector<int> C(N);
	setRand(A);
	std::copy(A.begin(), A.end(), C.begin());
	// my sortHeap
	{
		steady_clock::time_point start = steady_clock::now();
		buildHeap(A, A.size());
		sortHeap(A);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "sortHeap:\tA[" << A.size() << "] : ";
	print(A);
	// std::sort_heap
	{
		steady_clock::time_point start = steady_clock::now();
		std::make_heap(C.begin(), C.end());
		std::sort_heap(C.begin(), C.end());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "std::sort_heap:\tC[" << C.size() << "] : ";
	print(C);
	// diff
	cout << "equal(A,C) = " << std::boolalpha << std::equal(A.begin(), A.end(), C.begin()) << endl;
	// reverse order with float
	std::vector<float> AA(N);
	std::vector<float> CC(N);
	setRand(AA);
	std::copy(AA.begin(), AA.end(), CC.begin());
	// my sortHeap
	{
		steady_clock::time_point start = steady_clock::now();
		buildHeap(AA, AA.size(), std::greater());
		sortHeap(AA, std::greater());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "sortHeap:\tAA[" << AA.size() << "] : ";
	print(AA);
	// std::make_heap
	{
		steady_clock::time_point start = steady_clock::now();
		std::make_heap(CC.begin(), CC.end(), std::greater());
		std::sort_heap(CC.begin(), CC.end(), std::greater());
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "sort_heap:\tCC[" << CC.size() << "] : ";
	print(CC);
	// diff
	cout << "equal(AA,CC) = " << std::boolalpha << std::equal(AA.begin(), AA.end(), CC.begin()) << endl;
	// done
	return 0;
}
