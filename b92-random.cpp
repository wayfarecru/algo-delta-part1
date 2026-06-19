#include <iostream>
#include <iomanip>
#include <random>
using namespace std;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

int main(void) {
	std::random_device rd; // random device
	std::default_random_engine re; // random engine
	re.seed(rd());
	// uniform distribution test
	const int N = 1000000;
	const int M = 20;
	std::uniform_int_distribution<int> unif_dist(0,M-1);
	std::vector<int> A(M);
	for (int i = 0; i < N; ++i) {
		int ind = unif_dist(re);
		A[ind]++;
	}
	cout << "uniform distribution test with " << N << " trials into A[" << A.size() << "]" << endl;
	for (int i = 0; i < A.size(); ++i) {
		cout << "A[" << setw(2) << i << "] = " << setw(8) << A[i] << endl;
	}
	// normal distribution test
	std::normal_distribution<> normal_dist(10, 2);
	std::fill(A.begin(), A.end(), 0); // set A[0..M) to zero
	for (int i = 0; i < N; ++i) {
		int ind = std::round(normal_dist(re));
		if (0 <= ind && ind < A.size()) {
			A[ind]++;
		}
	}
	cout << "normal distribution test with " << N << " trials into A[" << A.size() << "]" << endl;
	for (int i = 0; i < A.size(); ++i) {
		cout << "A[" << setw(2) << i << "] = " << setw(8) << A[i] << endl;
	}
}
