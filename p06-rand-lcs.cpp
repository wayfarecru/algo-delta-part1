#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <cstring>
using namespace std;
using namespace std::chrono;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif


std::random_device rd; // random device
std::default_random_engine re(rd()); // random engine

std::string gen_rand_str(int n, int lo, int hi) { // generate random string of lenth n, in ASCII range [lo, hi]
	std::uniform_int_distribution<int> unif_dist(lo, hi);
	std::string str;
	while (n--) {
		int ch =unif_dist(re);
		str.append(1, ch);
	}
	return str;
}

int parse_num(int argc, char* argv[], int num = 30) {
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

std::vector<std::vector<int>> lcs_C; // works as a 2D array

void my_lcs_init(int lenA, int lenB) { // string lengths of A and B
	lcs_C.resize(lenA + 1);
	for (int i = 0; i < lenA + 1; ++i) {
		lcs_C[i].resize(lenB + 1);
		lcs_C[i][0] = 0;
	}
	memset(lcs_C[0].data(), 0, sizeof(int) * (lenB + 1));
}

void my_lcs_print(const std::string& A, std::string& S, int m, int n) {
	if (lcs_C[m][n] == 0) return;
	if (lcs_C[m][n] == lcs_C[m - 1][n]) {
		my_lcs_print(A, S, m - 1, n);
	} else if (lcs_C[m][n] == lcs_C[m][n - 1]) {
		my_lcs_print(A, S, m, n - 1);
	} else {
		my_lcs_print(A, S, m - 1, n - 1);
		S.push_back(A[m - 1]);
	}
}

std::string my_lcs(const std::string& A, const std::string& B) {
	my_lcs_init(A.length(), B.length());
	for (int i = 1; i < A.length() + 1; ++i) {
		for (int j = 1; j < B.length() + 1; ++j) {
			if (A[i - 1] != B[j - 1]) {
				lcs_C[i][j] = std::max( lcs_C[i][j - 1], lcs_C[i - 1][j] );
			} else {
				lcs_C[i][j] = lcs_C[i - 1][j - 1] + 1;
			}
		}
	}
	std::string S;
	my_lcs_print(A, S, A.length(), B.length());
	return S;
}

int main(int argc, char* argv[]) {
	int n = parse_num(argc, argv, 100);
	std::string A = gen_rand_str(n, 'a', 'z');
	std::string B = gen_rand_str(n, 'a', 'z');
	std::string S = my_lcs(A, B);
	cout << "A = " << A << endl;
	cout << "B = " << B << endl;
	cout << "LCS(A,B) = " << S << endl;
	cout << "length(LCS(A,B)) = " << S.length() << endl;
	// done
	return 0;
}
