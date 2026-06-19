#include <iostream>
#include <random>
#include <chrono>
#include <algorithm> // for stable_sort
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

template<typename T, typename Compare = std::less<T>>
void merge(std::vector<T>& A, int p, int q, int r, const Compare& compare = Compare()) {
	std::vector<T> L(&A[p], &A[q]); // copy A[p..q) to L[0..L.size)
	std::vector<T> R(&A[q], &A[r]); // copy A[q..r) to R[0..R.size)
	// merge L and R to A[p..r)
	int i = 0; // index for L
	int j = 0; // index for R
	int k = p; // index to A[p..r)
	while ((i < L.size()) && (j < R.size())) {
		if (compare(L[i], R[j])) {
			A[k++] = L[i++];
		} else {
			A[k++] = R[j++];
		}
	}
	// copy remaining things to the end of A
	std::copy(&L[i], &L[L.size()], &A[k]);
	std::copy(&R[j], &R[R.size()], &A[k]);
}

template<typename T, typename Compare = std::less<T>>
void merge_sort(std::vector<T>& A, int p, int r, const Compare& compare = Compare()) {
	if ((r - p) <= 1) return; // if 0 or 1 element, just return
	int q = (p + r) / 2; // mid point
	merge_sort(A, p, q, compare); // sort A[p..q)
	merge_sort(A, q, r, compare); // sort A[q..r)
	merge(A, p, q, r, compare);
}

void setRand(std::vector<int>& v, std::default_random_engine& re) {
	std::uniform_int_distribution<int> unif_dist(0, 999);
	for (int i = 0; i < v.size(); ++i) {
		v[i] = unif_dist(re);
	}
}

void setRand(std::vector<float>& v, std::default_random_engine& re) {
	std::uniform_real_distribution<float> unif_dist(0.0F, 1.0F);
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
	std::vector<int> A(N);
	std::vector<int> C(N);
	std::random_device rd; // random device
	std::default_random_engine re(rd()); // random engine
	setRand(A, re);
	std::copy(A.begin(), A.end(), C.begin());
	// merge_sort
	cout << "before:\t\tA[" << A.size() << "] : ";
	print(A);
	{
		steady_clock::time_point start = steady_clock::now();
		merge_sort(A, 0, N);
		steady_clock::time_point end = steady_clock::now();
		long long elapsed_msec = duration_cast<milliseconds>(end - start).count();
		cout << "elapsed time = " << elapsed_msec << " msec" << endl;
	}
	cout << "merge_sort:\tA[" << A.size() << "] : ";
	print(A);
	// stable_sort
	cout << "before:\t\tC[" << C.size() << "] : ";
	print(C);
	{
		steady_clock::time_point start = steady_clock::now();
		std::stable_sort(C.begin(), C.end());
		steady_clock::time_point end = steady_clock::now();
		long long elapsed_msec = duration_cast<milliseconds>(end - start).count();
		cout << "elapsed time = " << elapsed_msec << " msec" << endl;
	}
	cout << "stable_sort:\tC[" << C.size() << "] : ";
	print(C);
	// diff
	if (std::equal(A.begin(), A.end(), C.begin())) {
		cout << "identical" << endl;
	} else {
		cout << "different ****************" << endl;
	}
	// with float
	std::vector<float> AA(N);
	std::vector<float> CC(N);
	setRand(AA, re);
	std::copy(AA.begin(), AA.end(), CC.begin());
	// merget_sort
	cout << "before:\t\tAA[" << AA.size() << "] : ";
	print(AA);
	{
		steady_clock::time_point start = steady_clock::now();
		merge_sort(AA, 0, N);
		steady_clock::time_point end = steady_clock::now();
		long long elapsed_msec = duration_cast<milliseconds>(end - start).count();
		cout << "elapsed time = " << elapsed_msec << " msec" << endl;
	}
	cout << "merge_sort:\tAA[" << AA.size() << "] : ";
	print(AA);
	// stable_sort
	cout << "before:\t\tCC[" << CC.size() << "] : ";
	print(CC);
	{
		steady_clock::time_point start = steady_clock::now();
		std::stable_sort(CC.begin(), CC.end());
		steady_clock::time_point end = steady_clock::now();
		long long elapsed_msec = duration_cast<milliseconds>(end - start).count();
		cout << "elapsed time = " << elapsed_msec << " msec" << endl;
	}
	cout << "stable_sort:\tCC[" << CC.size() << "] : ";
	print(CC);
	// diff
	if (std::equal(AA.begin(), AA.end(), CC.begin())) {
		cout << "identical" << endl;
	} else {
		cout << "different ****************" << endl;
	}
	// done
	return 0;
}
