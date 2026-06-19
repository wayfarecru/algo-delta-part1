#include <iostream>
#include <vector>
#include <map>
#include <limits>
using namespace std;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

int mat_chain_rec(const std::vector<int>& p, int i, int j) {
	if (i == j) return 0;
	int k_sel = -1; // selected
	int best = std::numeric_limits<int>::max(); // current best score
	for (int k = i; k < j; ++k) {
		int cost_l = mat_chain_rec(p, i, k); // m[i,k]
		int cost_r = mat_chain_rec(p, k + 1, j); // m[k+1,j]
		int cost_m = p[i] * p[k + 1] * p[j + 1];
		int sum = cost_l + cost_r + cost_m;
		if (sum < best) {
			best = sum;
			k_sel = k;
		}
	}
	cout << "\tm[" << i << "," << j << "] = " << best << ", with k=" << k_sel;
	cout << ", choosing k[" << i << "," << k_sel << "] and k[" << k_sel + 1 << "," << j << "]" << endl;
	return best;
}

static std::map<std::pair<int,int>, std::pair<int,int>> mat_chain_M; // (i,j) -> (cost,k_sel)

int mat_chain_map(const std::vector<int>& p, int i, int j) {
	auto it = mat_chain_M.find(make_pair(i, j));
	if (it != mat_chain_M.end()) {
		return (it->second).first; // cost
	}
	if (i == j) {
		mat_chain_M[make_pair(i, i)] = make_pair(0, i);
		return 0;
	}
	int k_sel = -1; // selected
	int best = std::numeric_limits<int>::max(); // current best score
	for (int k = i; k < j; ++k) {
		int cost_l = mat_chain_map(p, i, k); // m[i,k]
		int cost_r = mat_chain_map(p, k + 1, j); // m[k+1,j]
		int cost_m = p[i] * p[k + 1] * p[j + 1]; // num of new mult
		int sum = cost_l + cost_r + cost_m;
		if (sum < best) {
			best = sum;
			k_sel = k;
		}
	}
	mat_chain_M[make_pair(i, j)] = make_pair(best, k_sel);
	return best;
}

void mat_chain_map_print(int i, int j) {
	if (i == j) {
		cout << "A[" << i << "]";
		return;
	}
	auto it = mat_chain_M.find(make_pair(i, j));
	int k_sel = (it->second).second;
	cout << ((i == k_sel) ? "" : "(");
	mat_chain_map_print(i, k_sel);
	cout << ((i == k_sel) ? "" : ")");
	cout << " x ";
	cout << ((k_sel + 1 == j) ? "" : "(");
	mat_chain_map_print(k_sel + 1, j);
	cout << ((k_sel + 1 == j) ? "" : ")");
}

static std::vector<std::vector<std::pair<int, int>>> mat_chain_V; // works as a 2D array

void mat_chain_init(int n) {
	mat_chain_V.resize(n);
	for (int i = 0; i < n; ++i) {
		mat_chain_V[i].resize(n);
		mat_chain_V[i][i] = make_pair(0, i);
	}
}

int mat_chain_bottomup(const std::vector<int>& p) {
	int n = p.size();
	mat_chain_init(n);
	for (int l = 2; l < n; ++l) {
		for (int i = 0; i < n - l; ++i) {
			int j = i + l - 1;
			int k_sel = -1; // selected
			int best = std::numeric_limits<int>::max(); // current best score
			for (int k = i; k < j; ++k) {
				int cost_l = mat_chain_map(p, i, k); // m[i,k]
				int cost_r = mat_chain_map(p, k + 1, j); // m[k+1,j]
				int cost_m = p[i] * p[k + 1] * p[j + 1];
				int sum = cost_l + cost_r + cost_m;
				if (sum < best) {
					best = sum;
					k_sel = k;
				}
			}
			mat_chain_V[i][j] = make_pair(best, k_sel); // memorize (cost,k_sel) pair
		}
	}
	return mat_chain_V[0][n-2].first; // from (cost,k_sel) pair
}

void mat_chain_bottomup_print(int i, int j) {
	if (i == j) {
		cout << "A[" << i << "]";
		return;
	}
	int k_sel = mat_chain_V[i][j].second; // from (cost,k_sel) pair
	cout << ((i == k_sel) ? "" : "(");
	mat_chain_bottomup_print(i, k_sel);
	cout << ((i == k_sel) ? "" : ")");
	cout << " x ";
	cout << ((k_sel + 1 == j) ? "" : "(");
	mat_chain_bottomup_print(k_sel + 1, j);
	cout << ((k_sel + 1 == j) ? "" : ")");
}

int main(int argc, char* argv[]) {
	std::vector<int> p = { 30, 35, 15, 5, 10, 20, 25 }; // example from CLRS
	// matrix chain multiplication
	cout << "mat-chain with recursion" << endl;
	mat_chain_rec(p, 0, p.size() - 2);
	//
	cout << "mat-chain top-down with map" << endl;
	int cost = mat_chain_map(p, 0, p.size() - 2);
	cout << "\tcost = " << cost << endl;
	cout << "\t";
	mat_chain_map_print(0, p.size() - 2);
	cout << endl;
	//
	cout << "mat-chain bottom-up" << endl;
	cost = mat_chain_bottomup(p);
	cout << "\tcost = " << cost << endl;
	cout << "\t";
	mat_chain_bottomup_print(0, p.size() - 2);
	cout << endl;
	// dump the 2D table, for the cost
	cout << "----" << endl;
	for (int i = 0; i < p.size() - 1; ++i) {
		for (int j = 0; j < p.size() - 1; ++j) {
			cout << mat_chain_V[i][j].first << "\t";
		}
		cout << endl;
	}
	// dump the 2D table, for the k_sel
	cout << "----" << endl;
	for (int i = 0; i < p.size() - 1; ++i) {
		for (int j = 0; j < p.size() - 1; ++j) {
			cout << mat_chain_V[i][j].second << "\t";
		}
		cout << endl;
	}
	// done
	return 0;
}
