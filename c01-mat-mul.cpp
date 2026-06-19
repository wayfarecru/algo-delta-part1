#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
using namespace std;
using namespace std::chrono;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

const int MAX_N = (4 * 1024);
// we will calculate C = A * B
float A[MAX_N][MAX_N];
float B[MAX_N][MAX_N];
float C[MAX_N][MAX_N];

void printMat(int n, const float M[MAX_N][MAX_N]) {
	if (n <= 8) {
		for (size_t r = 0; r < n; ++r) { // rows
			for (size_t c = 0; c < n; ++c) { // columns
				cout << std::fixed << setw(13) << std::setprecision(6) << M[r][c] << " ";
			}
			cout << endl;
		}
	} else { // 0 1 2 3 . . . 996 997 998 999
		for (size_t r = 0; r < 4; ++r) { // rows
			for (size_t c = 0; c < 4; ++c) { // columns
				cout << std::fixed << setw(13) << std::setprecision(6) << M[r][c] << " ";
			}
			cout << setw(13) << ". . ." << " ";
			for (size_t c = n - 4; c < n; ++c) { // columns
				cout << std::fixed << setw(13) << std::setprecision(6) << M[r][c] << " ";
			}
			cout << endl;
		}
		for (size_t r = 0; r < 9; ++r) {
			cout << setw(13) << ". . ." << " ";
		}
		cout << endl;
		for (size_t r = n - 4; r < n; ++r) { // rows
			for (size_t c = 0; c < 4; ++c) { // columns
				cout << std::fixed << setw(13) << std::setprecision(6) << M[r][c] << " ";
			}
			cout << setw(13) << ". . ." << " ";
			for (size_t c = n - 4; c < n; ++c) { // columns
				cout << std::fixed << setw(13) << std::setprecision(6) << M[r][c] << " ";
			}
			cout << endl;
		}
	}
}

void setRandMat(int n, float M[MAX_N][MAX_N], std::default_random_engine& re) {
	std::uniform_real_distribution<float> unif_dist(0.0F, 1.0F);
	for (size_t r = 0; r < n; ++r) {
		for (size_t c = 0; c < n; ++c) {
			M[r][c] = unif_dist(re);
		}
	}
}

void mat_mul(int n, float C[MAX_N][MAX_N], const float A[MAX_N][MAX_N], const float B[MAX_N][MAX_N]) {
	for (size_t r = 0; r < n; ++r) {
		for (size_t c = 0; c < n; ++c) {
			float val = 0.0F;
			for (size_t k = 0; k < n; ++k) {
				val += A[r][k] * B[k][c];
			}
			C[r][c] = val;
		}
	}
}

int main(int argc, char* argv[]) {
	int N = 1024;
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
	// optional error check
	if (N <= 0 || N > MAX_N) {
		cerr << argv[0] << ": illegal N=" << N << endl;
		exit(0);
	}
	// random numbers
	std::default_random_engine re; // random engine
	re.seed(0); // use the same seed for test purpose
	setRandMat(N, A, re);
	setRandMat(N, B, re);
	// mat_mul
	{
		steady_clock::time_point start = steady_clock::now();
		mat_mul(N, C, A, B);
		steady_clock::time_point end = steady_clock::now();
		long long elapsed_msec = duration_cast<milliseconds>(end - start).count();
		cout << "elapsed time = " << elapsed_msec << " msec" << endl;
	}
	// output
	cout << "A[" << N << "][" << N << "] = " << endl;
	printMat(N, A);
	cout << "B[" << N << "][" << N << "] = " << endl;
	printMat(N, B);
	cout << "C[" << N << "][" << N << "] = " << endl;
	printMat(N, C);
	// done
	return 0;
}
