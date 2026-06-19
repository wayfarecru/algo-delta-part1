#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <vector>
#include <queue> // for priority_queue
#include <algorithm>
#include <cassert>
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
		int child = 2 * (i + 1); // right child
		if ((child < n) && compare(A[largest], A[child])) {
			largest = child;
		}
		child--; // now, left child
		if (compare(A[largest], A[child])) {
			largest = child;
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
void pq_push(std::vector<T>& A, T val, const Compare& compare = Compare()) {
	A.push_back(val);
	pushHeap(A, A.size() - 1, compare);
}

template<typename T>
T pq_top(std::vector<T>& A) {
	assert(! A.empty());
	return A[0];
}

template<typename T>
bool pq_empty(std::vector<T>& A) {
	return A.empty();
}

template<typename T, typename Compare = std::less<T>>
void pq_pop(std::vector<T>& A, const Compare& compare = Compare()) {
	assert(! A.empty());
	A[0] = A.back();
	A.pop_back();
	heapify(A, A.size(), 0, compare);
}

void simul_pq_heap(int n, std::vector<int>& out) {
	std::uniform_int_distribution<int> unif_dist(0, 9999999);
	std::vector<int> PQ_HEAP;
	for (int i = 0; i < n; ++i) { // insert 3 times, consume once
		int val = unif_dist(re);
		pq_push(PQ_HEAP, val);
		if (i % 3 == 2) { // 0, 1, 2(consume), 3, 4, 5(consume), . . .
			out.push_back( pq_top(PQ_HEAP) );
			pq_pop(PQ_HEAP);
		}
	}
	// consume all remaining items
	while (! pq_empty(PQ_HEAP)) {
		out.push_back( pq_top(PQ_HEAP) );
		pq_pop(PQ_HEAP);
	}
}

void simul_pq(int n, std::vector<int>& out) {
	std::uniform_int_distribution<int> unif_dist(0, 9999999);
	std::priority_queue<int> PQ;
	for (int i = 0; i < n; ++i) { // insert 3 times, consume once
		int val = unif_dist(re);
		PQ.push( val );
		if (i % 3 == 2) { // 0, 1, 2(consume), 3, 4, 5(consume), . . .
			out.push_back( PQ.top() );
			PQ.pop();
		}
	}
	// consume all remaining items
	while (! PQ.empty()) {
		out.push_back( PQ.top() );
		PQ.pop();
	}
}

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 1000000);
	// simul with PQ_HEAP
	std::vector<int> C;
	re.seed(0);
	{
		steady_clock::time_point start = steady_clock::now();
		simul_pq_heap(N, C);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "pri-q on heap:\tC[" << C.size() << "] = ";
	print(C);
	// simul with std::priority_queue
	std::vector<int> A;
	re.seed(0);
	{
		steady_clock::time_point start = steady_clock::now();
		simul_pq(N, A);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "std::priority_queue:\tA[" << A.size() << "] : ";
	print(A);
	// diff
	cout << "equal(A,C) = " << std::boolalpha << std::equal(A.begin(), A.end(), C.begin()) << endl;
	// done
	return 0;
}
