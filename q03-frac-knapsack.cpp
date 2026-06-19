#include <iostream>
#include <vector>
#include <string>
#include <tuple> // for std::tie()
#include <algorithm>
using namespace std;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

static std::vector<std::string> item_name = { "wheat", "rice", "corn", "barley", "oats" };
static std::vector<float> item_w = { 12.0F, 15.0F, 20.0F, 15.0F, 14.0F }; // weight
static std::vector<float> item_v = { 50.0F, 50.0F, 40.0F, 80.0F, 30.0F }; // value

int main(int argc, char* argv[]) {
	// greedy fractional knapsack
	std::vector<std::pair<float, int >> item_ratio; // (ratio for value-per-weight, index)
	item_ratio.resize(item_w.size());
	for (int i = 0; i < item_w.size(); ++i) {
		item_ratio[i] = make_pair(item_v[i] / item_w[i], i);
	}
	std::sort(item_ratio.begin(), item_ratio.end(), [](const std::pair<float, int>& x, const std::pair<float, int>& y) {
		return (x.first > y.first); // reversely sorted, w.r.t value-per-weight ratio
	});
	// now greedy selections
	float C = 60.0F; // 60 kg knapsack
	float V = 0.0F; // total value
	cout << "target knapsack total weight = " << C << " kg" << endl;
	for (int i = 0; C > 0.0 && i < item_ratio.size(); ++i) {
		float ratio_sel; // value-per-weight ratio for i-th selected item
		float ind_sel; // index for i-th selected item
		std::tie(ratio_sel, ind_sel) = item_ratio[i];
		float w = (C > item_w[ind_sel]) ? item_w[ind_sel] : C;
		float v = ratio_sel * w;
		cout << "\titem " << item_name[ind_sel] << " : " << w << " kg selected for " << v << " dollars" << endl;
		C = C - w;
		V = V + v;
	}
	cout << "total value = " << V << " dollars" << endl;
	// done
	return 0;
}
