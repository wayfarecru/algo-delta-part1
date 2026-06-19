#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <vector>
#include <unordered_set>
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

template<typename HashType>
void print_hash(const HashType& s) {
	const int nElemPrint = 7; // number of printed elements, divided by 2
	int n = s.size();
	if (n < 2 * nElemPrint) {
		for (const auto& k : s) {
			cout << k << " ";
		}
	} else {
		int i = 0;
		for (const auto& k : s) {
			if ((i < nElemPrint) || (n - nElemPrint <= i)) {
				cout << k << " ";
			} else if (i == nElemPrint) {
				cout << ". . . ";
			}
			++i;
		}
	}
	cout << endl;
}

template<typename HashType>
void print_hash_details(const HashType& s) {
	const int nElemPrint = 7; // number of printed elements, divided by 2
	// print bucket sizes
	cout << "\tmax load factor = " << s.max_load_factor() << endl;
	cout << "\tcur load factor = " << s.load_factor() << endl;
	cout << "\tbucket_size[" << s.bucket_count() << "] = ";
	int m = s.bucket_count();
	if (m < 2 * nElemPrint) {
		for (int i = 0; i < m; ++i) {
			cout << s.bucket_size(i) << " ";
		}
	} else {
		for (int i = 0; i < nElemPrint; ++i) {
			cout << s.bucket_size(i) << " ";
		}
		cout << ". . . ";
		for (int i = m - nElemPrint; i < m; ++i) {
			cout << s.bucket_size(i) << " ";
		}
	}
	cout << endl;
	// print element, hash value, index
	auto h = s.hash_function();
	int n = s.size();
	if (n < 2 * nElemPrint) {
		for (const auto& k : s) {
			cout << "\tk = " << k << ",\th(k) = " << h(k) << ",\ti = " << s.bucket(k) << endl;
		}
	} else {
		int i = 0;
		for (const auto& k : s) {
			if ((i < nElemPrint) || (n - nElemPrint <= i)) {
				cout << "\tk = " << k << ",\th(k) = " << h(k) << ",\ti = " << s.bucket(k) << endl;
			} else if (i == nElemPrint) {
				cout << "\t. . . " << endl;
			}
			++i;
		}
	}
}

std::random_device rd; // random device
std::default_random_engine re(rd()); // random engine

template<typename HashType>
void set_rand_hash(HashType& s, int n = 1024) {
	using T = typename HashType::value_type;
	if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
		std::uniform_real_distribution<T> unif_dist(0.0F, 1.0F);
		for (int i = 0; i < n; ++i) {
			s.insert( unif_dist(re) );
		}
	} else {
		std::uniform_int_distribution<T> unif_dist(0, 9999999);
		for (int i = 0; i < n; ++i) {
			s.insert( unif_dist(re) );
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

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 1024);
	// hash table
	std::unordered_set<int> S;
	cout << "null S[" << S.size() << "] = ";
	print_hash(S);
	print_hash_details(S);
	// reserve many buckets
	S.max_load_factor(10.0);
	S.reserve(N);
	cout << "S[" << S.size() << "] = ";
	print_hash(S);
	print_hash_details(S);
	// insert elements
	re.seed(0);
	set_rand_hash(S, N);
	cout << "S[" << S.size() << "] = ";
	print_hash(S);
	print_hash_details(S);
	// again with multiset
	std::unordered_multiset<int> U;
	U.max_load_factor(10.0);
	U.reserve(N);
	re.seed(0);
	set_rand_hash(U, N);
	cout << "U[" << U.size() << "] = ";
	print_hash(U);
	print_hash_details(U);
	// done
	return 0;
}
