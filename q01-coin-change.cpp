#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <limits>
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

int parse_num(int argc, char* argv[], int num) {
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

static std::vector<int> den = { 50000, 10000, 5000, 1000, 500, 100, 50, 10, 5, 1 }; // must be sorted
// static std::vector<int> den = { 10000, 5000, 2000, 1000, 500, 100, 25, 10, 5, 1 }; // frequently used dollars, in cents

int main(int argc, char* argv[]) {
	int A = parse_num(argc, argv, 760); // target change
	cout << "target change A = " << A << endl;
	// greedy coin change
	for (int i = 0; A > 0; ++i) {
		int c = A / den[i];
		A = A - c * den[i];
		if (c > 0) {
			cout << "\t" << c << " x " << den[i] << " KRW" << endl;
		}
	}
	// done
	return 0;
}
