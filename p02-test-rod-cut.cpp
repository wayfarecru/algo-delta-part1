#include <iostream>
#include <vector>
#include <map>
#include <iterator>
using namespace std;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

std::vector<int> rod_p = { 0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30 }; // price p_i for i inches

int rod_cur(int n) {
	int m = rod_p.size() - 1;
	int r = 0;
	// int k_sel = 0; // if you need
	for (int i = 1; i <= std::min(m, n); ++i) {
		int r_cur = rod_p[i] + rod_cur(n - i);
		if (r_cur > r) {
			r = r_cur;
			// k_sel = i; // if you need
		}
	}
	return r;
}

std::map<int, std::pair<int,int>> rod_M; // i -> (r_i, k_sel)

int rod_map(int n) {
	auto it = rod_M.find(n);
	if (it != rod_M.end()) {
		return (it->second).first; // r_i, memorized value
	}
	int m = rod_p.size() - 1;
	int r = 0;
	int k_sel = 0;
	for (int i = 1; i <= std::min(m, n); ++i) {
		int r_cur = rod_p[i] + rod_map(n - i);
		if (r_cur > r) {
			r = r_cur;
			k_sel = i;
		}
	}
	rod_M[n] = make_pair(r, k_sel);
	return r;
}

void rod_cut(std::vector<int>& cut, int n) {
	int k_sel = rod_M[n].second;
	if (k_sel > 0) {
		cut.push_back(k_sel);
		rod_cut(cut, n - k_sel);
	} else if (n > 0) {
		cut.push_back(n);
	}
}

std::vector<std::pair<int,int>> rod_A; // (r, k_sel)

int rod_bottomup(int n) {
	rod_A.resize(n + 1);
	int m = rod_p.size() - 1;
	rod_A[0] = make_pair(0, 0);
	for (int j = 1; j <= n; ++j) {
		int r = 0;
		int k_sel = 0;
		for (int i = 1; i <= std::min(m, j); ++i) {
			int r_cur = rod_p[i] + rod_A[j - i].first;
			if (r_cur > r) {
				r = r_cur;
				k_sel = i;
			}
		}
		rod_A[j] = make_pair(r, k_sel);
	}
	return rod_A[n].first;
}

void rod_bottomup_cut(std::vector<int>& cut, int n) {
	int k_sel = rod_A[n].second;
	if (k_sel > 0) {
		cut.push_back(k_sel);
		rod_bottomup_cut(cut, n - k_sel);
	} else if (n > 0) {
		cut.push_back(n);
	}
}

int parse_num(int argc, char* argv[], int num = 1000) {
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

int main(int argc, char* argv[]) {
	int n = parse_num(argc, argv, 1000);
	// naive recursive implementation
	cout << "recursive cut" << endl;
	for (int i = 1; i <= 10; ++i) {
		cout << "\ti = " << i << "\tr_i = " << rod_cur(i) << endl;
	}
	// dynamic prog, top-down
	cout << "top down with map" << endl;
	for (int i = 1; i <= 10; ++i) {
		cout << "\ti = " << i << "\tr_i = " << rod_map(i) << ", with cutting at ";
		std::vector<int> cut;
		rod_cut(cut, i);
		std::copy(cut.begin(), cut.end(), std::ostream_iterator<int>(std::cout, " "));
		cout << endl;
	}
	// dynamic prog, bottom-up
	cout << "bottom up with array" << endl;
	rod_bottomup(10);
	for (int i = 1; i <= 10; ++i) {
		cout << "\ti = " << i << "\tr_i = " << rod_A[i].first << ", with cutting at ";
		std::vector<int> cut;
		rod_bottomup_cut(cut, i);
		std::copy(cut.begin(), cut.end(), std::ostream_iterator<int>(std::cout, " "));
		cout << endl;
	}
	// test for big size
	cout << "cutting with n = " << n << endl;
	rod_bottomup(n);
	cout << "\tr = " << rod_A[n].first << ", with cutting at ";
	std::vector<int> cut;
	rod_bottomup_cut(cut, n);
	std::copy(cut.begin(), cut.end(), std::ostream_iterator<int>(std::cout, " "));
	cout << endl;
	// done
	return 0;
}
