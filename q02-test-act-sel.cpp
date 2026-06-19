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

std::vector<std::pair<int, int>> act_sel_S; // storing (si,fi), activity set

void act_sel_init(void) { // as given in CLRS book
	// init the set
	act_sel_S.resize(12);
	act_sel_S[0] = make_pair(0, 0); // a_0 : dummy job for left bound
	act_sel_S[1] = make_pair(1, 4);
	act_sel_S[2] = make_pair(3, 5);
	act_sel_S[3] = make_pair(0, 6);
	act_sel_S[4] = make_pair(5, 7);
	act_sel_S[5] = make_pair(3, 9);
	act_sel_S[6] = make_pair(5, 9);
	act_sel_S[7] = make_pair(6, 10);
	act_sel_S[8] = make_pair(7, 11);
	act_sel_S[9] = make_pair(8, 12);
	act_sel_S[10] = make_pair(2, 14);
	act_sel_S[11] = make_pair(12, 16);
	act_sel_S.push_back(make_pair(std::numeric_limits<int>::max(), std::numeric_limits<int>::max())); // for right bound
#if 1 // if you need to sort them
	sort(act_sel_S.begin(), act_sel_S.end(), [](const std::pair<int, int>& x, const std::pair<int, int>& y) {
		return (x.second < y.second);
	});
#endif
}

std::map<std::pair<int,int>, std::pair<int,int>> act_sel_M; // (i,j) -> (num,k_sel)

int act_sel_rec(int i, int j) {
	auto it = act_sel_M.find(make_pair(i, j));
	if (it != act_sel_M.end()) {
		return (it->second).first; // cost c
	}
	if (i > j) {
		return 0;
	}
	// a[i-1] and a[j+1] are selected
	int left_bound = act_sel_S[i - 1].second;
	int right_bound = act_sel_S[j + 1].first;
	int num = 0; // count feasible activities
	for (int k = i; k <= j; ++k) {
		int s, f; // start and finish time of a_k
		std::tie(s, f) = act_sel_S[k];
		if ((left_bound <= s) && (f <= right_bound)) {
			num++;
		}
	}
	if (num == 0) {
		act_sel_M[make_pair(i, j)] = make_pair(0, 0);
		return 0;
	}
	// recursive evaluation
	int num_sel = 0;
	int k_sel = 0;
	for (int k = i; k <= j; ++k) {
		int s, f; // start and finish time of a_k
		std::tie(s, f) = act_sel_S[k];
		if ((left_bound <= s) && (f <= right_bound)) {
			int num = act_sel_rec(i, k - 1) + act_sel_rec(k + 1, j) + 1;
			if (num > num_sel) {
				num_sel = num;
				k_sel = k;
			}
		}
	}
	act_sel_M[make_pair(i, j)] = make_pair(num_sel, k_sel);
	return num_sel;
}

void act_sel_rec_get(std::vector<int>& ans, int i, int j) {
	auto it = act_sel_M.find(make_pair(i, j));
	if (it != act_sel_M.end()) {
		int k_sel = (it->second).second;
		if (k_sel != 0) {
			act_sel_rec_get(ans, i, k_sel - 1);
			ans.push_back(k_sel);
			act_sel_rec_get(ans, k_sel + 1, j);
		}
	}
}

std::vector<int> act_sel_dyn_prog(void) {
	int n = act_sel_S.size() - 2;
	act_sel_rec(1, n);
	std::vector<int> ans;
	act_sel_rec_get(ans, 1, n);
	return ans;
}

std::vector<int> act_sel_greedy(void) {
	int n = act_sel_S.size() - 2; // 0, 1, 2, . . . , n-1, n, n+1
	std::vector<int> ans;
	ans.push_back(1); // a_1 is always included
	int f_last = act_sel_S[1].second; // f_1 : finish time of a_1
	for (int i = 2; i <= n; ++i) {
		int s, f; // start and finish time of a_i
		std::tie(s, f) = act_sel_S[i];
		if (s >= f_last) {
			ans.push_back(i);
			f_last = f;
		}
	}
	return ans;
}

void act_sel_print(const std::vector<int>& S) {
	for (int i = 0; i < S.size(); ++i) {
		int ind = S[i];
		int s, f; // start and finish time of a_i
		std::tie(s, f) = act_sel_S[ind];
		cout << "\tact[" << ind << "] = (" << s << "," << f << ")" << endl;
	}
}

int main(int argc, char* argv[]) {
	act_sel_init();
	// dynamic programming
	cout << "dynamic prog:" << endl;
	std::vector<int> ans0 = act_sel_dyn_prog();
	act_sel_print(ans0);
	// greedy method
	cout << "greedy method:" << endl;
	std::vector<int> ans = act_sel_greedy();
	act_sel_print(ans);
	// done
	return 0;
}
