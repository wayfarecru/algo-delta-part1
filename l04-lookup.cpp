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
void print(const unordered_map<K, T>& L) {
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
void print_details(const unordered_map<K, T>& L) {
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

// --------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
	// lookup table
	std::unordered_map<std::string, uint32_t> L;
	L["hello"] = 0x01;
	L["world"] = 0x02;
	L["alpha"] = 0x03;
	L["bravo"] = 0x04;
	cout << "L[" << L.size() << "] = ";
	print(L);
	print_details(L);
	// done
	return 0;
}
