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

template<typename T>
void print_hash(const std::unordered_set<T>& s) {
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

template<typename T>
void print_hash_details(const std::unordered_set<T>& s) {
	const int nElemPrint = 7; // number of printed elements, divided by 2
	// print bucket sizes
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

template<typename T>
void print_hash(const std::unordered_multiset<T>& s) {
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

template<typename T>
void print_hash_details(const std::unordered_multiset<T>& s) {
	const int nElemPrint = 7; // number of printed elements, divided by 2
	// print bucket sizes
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

// --------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
	// unordered_set
	std::unordered_set<int> S;
	cout << "null set S[" << S.size() << "] = ";
	print_hash(S);
	print_hash_details(S);
	// insert elements
	std::vector<int> v { 2, 1, 7, 1, 9, 5, 28, 7 };
	for (auto& k : v) {
		S.insert(k);
	}
	cout << "set S[" << S.size() << "] = ";
	print_hash(S);
	print_hash_details(S);
	// unordered_multiset
	std::unordered_multiset<int> T { 2, 1, 7, 1, 9, 5, 28, 7 };
	cout << "multiset T[" << T.size() << "] = ";
	print_hash(T);
	print_hash_details(T);
	// done
	return 0;
}
