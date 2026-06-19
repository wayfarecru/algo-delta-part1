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
void merge(std::vector<T>& A, std::vector<T>& B, int p, int q, int r, const Compare& compare = Compare()) {
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
void merge_sort(std::vector<T>& A, std::vector<T>& B, int p, int r, const Compare& compare = Compare()) {
	if ((r - p) <= 1) return; // if 0 or 1 element, just return
	int q = (p + r) / 2; // mid point
	merge_sort(A, B, p, q, compare); // sort A[p..q)
	merge_sort(A, B, q, r, compare); // sort A[q..r)
	merge(A, B, p, q, r, compare);
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
	std::vector<int> B(N);
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
		merge_sort(A, B, 0, N, std::greater<>());
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
		std::stable_sort(C.begin(), C.end(), std::greater<>());
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
	std::vector<float> BB(N);
	std::vector<float> CC(N);
	setRand(AA, re);
	std::copy(AA.begin(), AA.end(), CC.begin());
	// merget_sort
	cout << "before:\t\tAA[" << AA.size() << "] : ";
	print(AA);
	{
		steady_clock::time_point start = steady_clock::now();
		merge_sort(AA, BB, 0, N, std::greater<>());
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
		std::stable_sort(CC.begin(), CC.end(), std::greater<>());
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
