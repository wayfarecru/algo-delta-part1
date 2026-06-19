#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <map>
#include <limits>
using namespace std;
using namespace std::chrono;
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

std::random_device rd; // random device
std::default_random_engine re(rd()); // random engine

template<typename T>
void set_rand(typename std::vector<T>::iterator first, typename std::vector<T>::iterator last) {
	if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
		std::uniform_real_distribution<T> unif_dist(0.0F, 1.0F);
		for (typename std::vector<T>::iterator it = first; it != last; ++it) {
			*it = unif_dist(re);
		}
	} else {
		std::uniform_int_distribution<T> unif_dist(10, 1000);
		for (typename std::vector<T>::iterator it = first; it != last; ++it) {
			*it = unif_dist(re);
		}
	}
}

template<typename T>
void set_rand(std::vector<T>& v) {
	set_rand<T>(v.begin(), v.end());
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

int main(int argc, char* argv[]) {
	int n = parse_num(argc, argv, 30);
	std::vector<int> p(n);
	set_rand(p);
	// matrix chain multiplication
	cout << "mat-chain with recursion:\t";
	{
		steady_clock::time_point start = steady_clock::now();
		int cost = mat_chain_rec(p, 0, p.size() - 2);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "\tcost = " << cost << endl;
	}
	//
	cout << "mat-chain top-down with map:\t";
	{
		steady_clock::time_point start = steady_clock::now();
		int cost = mat_chain_map(p, 0, p.size() - 2);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "\tcost = " << cost << endl;
	}
	cout << "\t";
	mat_chain_map_print(0, p.size() - 2);
	cout << endl;
	//
	cout << "mat-chain bottom-up:\t";
	{
		steady_clock::time_point start = steady_clock::now();
		int cost = mat_chain_bottomup(p);
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
		cout << "\tcost = " << cost << endl;
	}
	cout << "\t";
	mat_chain_bottomup_print(0, p.size() - 2);
	cout << endl;
	// done
	return 0;
}
