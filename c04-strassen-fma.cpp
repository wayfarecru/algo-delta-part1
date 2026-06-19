#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <cstring> // for memset()
#include <cassert>
using namespace std;
using namespace std::chrono;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

void print(const vector<vector<float>>& mat) {
	size_t n = mat.size();
	if (n <= 8) {
		for (size_t r = 0; r < n; ++r) { // rows
			for (size_t c = 0; c < n; ++c) { // columns
				cout << std::fixed << setw(13) << std::setprecision(6) << mat[r][c] << " ";
			}
			cout << endl;
		}
	} else { // 0 1 2 3 . . . 996 997 998 999
		for (size_t r = 0; r < 4; ++r) { // rows
			for (size_t c = 0; c < 4; ++c) { // columns
				cout << std::fixed << setw(13) << std::setprecision(6) << mat[r][c] << " ";
			}
			cout << setw(13) << ". . ." << " ";
			for (size_t c = n - 4; c < n; ++c) { // columns
				cout << std::fixed << setw(13) << std::setprecision(6) << mat[r][c] << " ";
			}
			cout << endl;
		}
		for (size_t r = 0; r < 9; ++r) {
			cout << setw(13) << ". . ." << " ";
		}
		cout << endl;
		for (size_t r = n - 4; r < n; ++r) { // rows
			for (size_t c = 0; c < 4; ++c) { // columns
				cout << std::fixed << setw(13) << std::setprecision(6) << mat[r][c] << " ";
			}
			cout << setw(13) << ". . ." << " ";
			for (size_t c = n - 4; c < n; ++c) { // columns
				cout << std::fixed << setw(13) << std::setprecision(6) << mat[r][c] << " ";
			}
			cout << endl;
		}
	}
}

void resize(vector<vector<float>>& mat, int n) {
	mat.resize(n);
	for (int i = 0; i < n; ++i) {
		mat[i].resize(n);
	}
}

void setRand(vector<vector<float>>& mat, default_random_engine& re) {
	uniform_real_distribution<float> unif_dist(0.0F, 1.0F);
	size_t n = mat.size();
	for (size_t r = 0; r < n; ++r) {
		for (size_t c = 0; c < n; ++c) {
			mat[r][c] = unif_dist(re);
		}
	}
}

void clear(vector<vector<float>>& mat) {
	size_t n = mat.size();
	for (size_t r = 0; r < n; ++r) {
		std::fill(mat[r].begin(), mat[r].end(), 0.0F);
	}
}

void divide(const vector<vector<float>>& mat,
            vector<vector<float>>& a11, vector<vector<float>>& a12,
            vector<vector<float>>& a21, vector<vector<float>>& a22) {  // mat -> (a11, a12 // a21, a22 )
	size_t n = mat.size();
	size_t m = n / 2;
	for (size_t r = 0; r < m; ++r) { std::copy(&(mat[r][0]), &(mat[r][m]), &(a11[r][0])); }
	for (size_t r = 0; r < m; ++r) { std::copy(&(mat[r][m]), &(mat[r][n]), &(a12[r][0])); }
	for (size_t r = 0; r < m; ++r) { std::copy(&(mat[r + m][0]), &(mat[r + m][m]), &(a21[r][0])); }
	for (size_t r = 0; r < m; ++r) { std::copy(&(mat[r + m][m]), &(mat[r + m][n]), &(a22[r][0])); }
}

void combine(vector<vector<float>>& mat,
             const vector<vector<float>>& a11, const vector<vector<float>>& a12,
             const vector<vector<float>>& a21, const vector<vector<float>>& a22) {  // mat <- (a11, a12 // a21, a22 )
	size_t n = mat.size();
	size_t m = n / 2;
	for (size_t r = 0; r < m; ++r) { std::copy(&(a11[r][0]), &(a11[r][m]), &(mat[r][0])); }
	for (size_t r = 0; r < m; ++r) { std::copy(&(a12[r][0]), &(a12[r][m]), &(mat[r][m])); }
	for (size_t r = 0; r < m; ++r) { std::copy(&(a21[r][0]), &(a21[r][m]), &(mat[r + m][0])); }
	for (size_t r = 0; r < m; ++r) { std::copy(&(a22[r][0]), &(a22[r][m]), &(mat[r + m][m])); }
}

void add(vector<vector<float>>& dst,
         const vector<vector<float>>& lhs, const vector<vector<float>>& rhs) { // dst = lhs + rhs
	size_t n = dst.size();
	for (size_t r = 0; r < n; ++r) {
		for (size_t c = 0; c < n; ++c) {
			dst[r][c] = lhs[r][c] + rhs[r][c];
		}
	}
}

void sub(vector<vector<float>>& dst,
         const vector<vector<float>>& lhs, const vector<vector<float>>& rhs) { // dst = lhs + rhs
	size_t n = dst.size();
	for (size_t r = 0; r < n; ++r) {
		for (size_t c = 0; c < n; ++c) {
			dst[r][c] = lhs[r][c] - rhs[r][c];
		}
	}
}

void fma(vector<vector<float>>& dst,
         const vector<vector<float>>& lhs, const vector<vector<float>>& rhs) { // dst += lhs * rhs
	size_t n = dst.size();
	if (n == 1) {
		dst[0][0] += lhs[0][0] * rhs[0][0];
		return;
	}
	size_t m = n / 2;
	vector<vector<float>> a11, a12, a21, a22;
	resize(a11, m); resize(a12, m); resize(a21, m); resize(a22, m);
	vector<vector<float>> b11, b12, b21, b22;
	resize(b11, m); resize(b12, m); resize(b21, m); resize(b22, m);
	vector<vector<float>> c11, c12, c21, c22;
	resize(c11, m); resize(c12, m); resize(c21, m); resize(c22, m);
	divide(lhs, a11, a12, a21, a22);
	divide(rhs, b11, b12, b21, b22);
	divide(dst, c11, c12, c21, c22);
	vector<vector<float>> s1, s2, s3, s4, s5, s6, s7, s8, s9, s10;
	resize(s1, m); resize(s2, m); resize(s3, m); resize(s4, m);
	resize(s5, m); resize(s6, m); resize(s7, m); resize(s8, m);
	resize(s9, m); resize(s10, m);
	vector<vector<float>> p1, p2, p3, p4, p5, p6, p7;
	resize(p1, m); resize(p2, m); resize(p3, m); resize(p4, m);
	resize(p5, m); resize(p6, m); resize(p7, m);
	clear(p1); clear(p2); clear(p3); clear(p4);
	clear(p5); clear(p6); clear(p7);
	sub(s1, b12, b22); // s1 = b12 - b22;
	add(s2, a11, a12); // s2 = a11 + a12;
	add(s3, a21, a22); // s3 = a21 + a22;
	sub(s4, b21, b11); // s4 = b21 - b11;
	add(s5, a11, a22); // s5 = a11 + a22;
	add(s6, b11, b22); // s6 = b11 + b22;
	sub(s7, a12, a22); // s7 = a12 - a22;
	add(s8, b21, b22); // s8 = b21 + b22;
	sub(s9, a11, a21); // s9 = a11 - a21;
	add(s10, b11, b12); // s10 = b11 + b12;
	fma(p1, a11, s1);
	fma(p2, s2, b22);
	fma(p3, s3, b11);
	fma(p4, a22, s4);
	fma(p5, s5, s6);
	fma(p6, s7, s8);
	fma(p7, s9, s10);
	add(c11, c11, p5);
	add(c11, c11, p4);
	sub(c11, c11, p2);
	add(c11, c11, p6);
	add(c12, c12, p1);
	add(c12, c12, p2);
	add(c21, c21, p3);
	add(c21, c21, p4);
	add(c22, c22, p5);
	add(c22, c22, p1);
	sub(c22, c22, p3);
	sub(c22, c22, p7);
	combine(dst, c11, c12, c21, c22);
}

int main(int argc, char* argv[]) {
	const int MAX_N = (4 * 1024);
	int N = 256;
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
	if ((N & (N - 1)) != 0) { // check for power of 2
		cerr << argv[0] << ": not a power of 2: N=" << N << endl;
		exit(0);
	}
	// prepare random matrices
	vector<vector<float>> A, B, C;
	resize(A, N);
	resize(B, N);
	resize(C, N);
	std::default_random_engine re; // random engine
	re.seed(0);
	setRand(A, re);
	setRand(B, re);
	clear(C);
	// recursive mat mul
	{
		steady_clock::time_point start = steady_clock::now();
		fma(C, A, B);
		steady_clock::time_point end = steady_clock::now();
		long long elapsed_msec = duration_cast<milliseconds>(end - start).count();
		cout << "elapsed time = " << elapsed_msec << " msec" << endl;
	}
	// output
	cout << "A[" << N << "][" << N << "] = " << endl;
	print(A);
	cout << "B[" << N << "][" << N << "] = " << endl;
	print(B);
	cout << "C[" << N << "][" << N << "] = " << endl;
	print(C);
	// done
	return 0;
}
