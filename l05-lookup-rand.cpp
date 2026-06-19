#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <vector>
#include <unordered_set>
#include <unordered_map>
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

template<typename K, typename T>
void print(const std::unordered_map<K, T>& L) {
	const int nElemPrint = 7; // number of printed elements, divided by 2
	int n = L.size();
	if (n < 2 * nElemPrint) {
		for (const auto& p : L) {
			cout << "(" << p.first << "," << p.second << ") ";
		}
	} else {
		int i = 0;
		for (const auto& p : L) {
			if ((i < nElemPrint) || (n - nElemPrint <= i)) {
				cout << "(" << p.first << "," << p.second << ") ";
			} else if (i == nElemPrint) {
				cout << ". . . ";
			}
			++i;
		}
	}
	cout << endl;
}

template<typename K, typename T>
void print_details(const std::unordered_map<K, T>& L) {
	const int nElemPrint = 7; // number of printed elements, divided by 2
	// print bucket sizes
	cout << "\tmax load factor = " << L.max_load_factor() << endl;
	cout << "\tcur load factor = " << L.load_factor() << endl;
	cout << "\tbucket_size[" << L.bucket_count() << "] = ";
	int m = L.bucket_count();
	if (m < 2 * nElemPrint) {
		for (int i = 0; i < m; ++i) {
			cout << L.bucket_size(i) << " ";
		}
	} else {
		for (int i = 0; i < nElemPrint; ++i) {
			cout << L.bucket_size(i) << " ";
		}
		cout << ". . . ";
		for (int i = m - nElemPrint; i < m; ++i) {
			cout << L.bucket_size(i) << " ";
		}
	}
	cout << endl;
	// print element, hash value, index
	auto h = L.hash_function();
	int n = L.size();
	if (n < 2 * nElemPrint) {
		for (const auto& p : L) {
			auto k = p.first;
			cout << "\tk = " << k << ",\th(k) = " << h(k) << ",\ti = " << L.bucket(k) << endl;
		}
	} else {
		int i = 0;
		for (const auto& p : L) {
			if ((i < nElemPrint) || (n - nElemPrint <= i)) {
				auto k = p.first;
				cout << "\tk = " << k << ",\th(k) = " << h(k) << ",\ti = " << L.bucket(k) << endl;
			} else if (i == nElemPrint) {
				cout << "\t. . . " << endl;
			}
			++i;
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

std::random_device rd; // random device
std::default_random_engine re(rd()); // random engine

void set_rand_str(std::unordered_map<std::string, uint32_t>& L, int n = 1024) {
	std::uniform_int_distribution<int> unif_dist(0, 9999999);
	// generate strings and numbers
	for (int i = 0; i < n; ++i) {
		std::string str;
		str.clear();
		int len = (unif_dist(re) % 8) + 2; // string lengh 2 to 9
		for (int j = 0; j < len; ++j) {
			char ch;
			do {
				ch = (unif_dist(re) % 58) + 65; // ASCII code: 'A'(65) to 'z'(122)
			} while (! isalpha(ch));
			str.append(1, ch);
		}
		uint32_t num = unif_dist(re);
		L[str] = num;
	}
}

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 1024);
	// lookup table
	std::unordered_map<std::string, uint32_t> L;
	L.max_load_factor(8.0);
	set_rand_str(L, N);
	cout << "L[" << L.size() << "] = ";
	print(L);
	print_details(L);
	// done
	return 0;
}
