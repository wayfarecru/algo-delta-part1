#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <cassert>
using namespace std;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

std::vector<float> p = { 0.00, 0.15, 0.10, 0.05, 0.10, 0.20 }; // as in CLRS book
std::vector<float> q = { 0.05, 0.10, 0.05, 0.05, 0.05, 0.10 };

float opt_bst_rec_w(int i, int j) {
	if (j < i) {
		assert(j == (i - 1));
		return q[i - 1];
	}
	return opt_bst_rec_w(i, j - 1) + p[j] + q[j];
}

float opt_bst_rec(int i, int j) {
	if (j < i) {
		assert(j == (i - 1));
		return q[i - 1];
	}
	int r_sel = -1; // selected
	float best = std::numeric_limits<float>::max(); // current best score
	float cost_m = opt_bst_rec_w(i, j);
	for (int r = i; r <= j; ++r) {
		float cost_l = opt_bst_rec(i, r - 1);
		float cost_r = opt_bst_rec(r + 1, j);
		float sum = cost_l + cost_r + cost_m;
		if (sum < best) {
			best = sum;
			r_sel = r;
		}
	}
	cout << "\te[" << i << "," << j << "] = " << best << ", with r=" << r_sel;
	cout << ", choosing e[" << i << "," << r_sel << "] and e[" << r_sel + 1 << "," << j << "]" << endl;
	return best;
}

static std::map<std::pair<int, int>, float> opt_bst_M_w; // w[i,j]
static std::map<std::pair<int, int>, std::pair<float, int>> opt_bst_M_e; // (i,j) -> (e[i,j], r_sel)

float opt_bst_map_w(int i, int j) {
	auto it = opt_bst_M_w.find(make_pair(i, j));
	if (it != opt_bst_M_w.end()) {
		return it->second;
	}
	if (j < i) {
		assert(j == (i - 1));
		return (opt_bst_M_w[make_pair(i, j)] = q[i - 1]);
	}
	return (opt_bst_M_w[make_pair(i, j)] = opt_bst_rec_w(i, j - 1) + p[j] + q[j]);
}

float opt_bst_map(int i, int j) {
	auto it = opt_bst_M_e.find(make_pair(i, j));
	if (it != opt_bst_M_e.end()) {
		return (it->second).first; // best cost
	}
	if (j < i) {
		assert(j == (i - 1));
		opt_bst_M_e[make_pair(i, j)] = make_pair(q[i - 1], -1);
		return q[i - 1];
	}
	int r_sel = -1; // selected
	float best = std::numeric_limits<float>::max(); // current best score
	float cost_m = opt_bst_map_w(i, j);
	for (int r = i; r <= j; ++r) {
		float cost_l = opt_bst_map(i, r - 1);
		float cost_r = opt_bst_map(r + 1, j);
		float sum = cost_l + cost_r + cost_m;
		if (sum < best) {
			best = sum;
			r_sel = r;
		}
	}
	opt_bst_M_e[make_pair(i, j)] = make_pair(best, r_sel);
	return best;
}

void opt_bst_map_print(int i, int j) {
	if (i == j) {
		cout << "[" << i << "]";
		return;
	}
	if (j < i) { 
		cout << "[empty]";
		return;
	}
	auto it = opt_bst_M_e.find(make_pair(i, j));
	int r_sel = (it->second).second;
	if (r_sel == 0) return;
	cout << "[" << r_sel << "]"; // root node first
	cout << " (";
	opt_bst_map_print(i, r_sel - 1);
	cout << ") ";
	cout << " (";
	opt_bst_map_print(r_sel + 1, j);
	cout << ") ";
}

static std::vector<std::vector<float>> opt_bst_V_w; // w[i,j]
static std::vector<std::vector<float>> opt_bst_V_e; // e[i,j]
static std::vector<std::vector<int>> opt_bst_V_r; // r[i,j]

void opt_bst_init(int n) {
	opt_bst_V_w.resize(n + 1); // we need [0..n]
	opt_bst_V_e.resize(n + 1); // we need [0..n]
	opt_bst_V_r.resize(n + 1); // we need [0..n]
	for (int i = 0; i <= n; ++i) {
		opt_bst_V_w[i].resize(n+1);
		opt_bst_V_e[i].resize(n+1);
		opt_bst_V_r[i].resize(n+1);
	}
	for (int i = 1; i <= n; ++i) {
		opt_bst_V_w[i][i-1] = q[i-1];
		opt_bst_V_e[i][i-1] = q[i-1];
		opt_bst_V_r[i][i-1] = -1;
	}
}

float opt_bst_bottomup(int n) {
	opt_bst_init(n + 1);
	for (int l = 1; l <= n + 1; ++l) {
		for (int i = 1; i <= n - l + 1; ++i) {
			int j = i + l - 1;
			int r_sel = -1; // selected
			float best = std::numeric_limits<int>::max(); // current best score
			opt_bst_V_w[i][j] = opt_bst_V_w[i][j - 1] + p[j] + q[j];
			for (int r = i; r <= j; ++r) {
				float cost_l = opt_bst_V_e[i][r - 1];
				float cost_r = opt_bst_V_e[r + 1][j];
				float cost_m = opt_bst_V_w[i][j];
				float sum = cost_l + cost_r + cost_m;
				if (sum < best) {
					best = sum;
					r_sel = r;
				}
			}
			opt_bst_V_e[i][j] = best;
			opt_bst_V_r[i][j] = r_sel;
		}
	}
	return opt_bst_V_e[1][n];
}

int main(int argc, char* argv[]) {
	// optimal BST, recursive implementation
	cout << "optimal BST with recursion" << endl;
	float cost = opt_bst_rec(1, p.size() - 1);
	cout << "\tbest cost = " << cost << endl;
	// 
	cout << "optimal BST top-down with map" << endl;
	cost = opt_bst_map(1, p.size() - 1);
	cout << "\tbest cost = " << cost << endl;
	cout << "\t";
	opt_bst_map_print(1, p.size() - 1);
	cout << endl;
	// 
	cout << "optimal BST bottom-up" << endl;
	int n = p.size() - 1;
	float cost2 = opt_bst_bottomup(n);
	cout << "\tbest cost = " << (float)cost2 << endl;
	// dump the 2D table, for the cost
	cout << "w ----" << endl;
	for (int i = 1; i <= n+1; ++i) {
		for (int j = 0; j <= n; ++j) {
			cout << opt_bst_V_w[i][j] << "\t";
		}
		cout << endl;
	}
	// dump the 2D table, for the cost
	cout << "e ----" << endl;
	for (int i = 1; i <= n+1; ++i) {
		for (int j = 0; j <= n; ++j) {
			cout << opt_bst_V_e[i][j] << "\t";
		}
		cout << endl;
	}
	// dump the 2D table, for the cost
	cout << "r ----" << endl;
	for (int i = 1; i <= n+1; ++i) {
		for (int j = 0; j <= n; ++j) {
			cout << opt_bst_V_r[i][j] << "\t";
		}
		cout << endl;
	}
	// done
	return 0;
}
