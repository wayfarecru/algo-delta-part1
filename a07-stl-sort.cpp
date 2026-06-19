#include <iostream>
#include <vector>
#include <algorithm> // for sort()
using namespace std;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

template<typename T>
void print(const std::vector<T>& v) {
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;
}

bool cmp(const float a, const float b) {
	return (a > b);
}

int main(void) {
	// int
	std::vector<int> A = { 3, 4, 1, 2, 5, 8, 6, 7, 9, 0 };
	std::vector<int> B(A);
	print(A);
	std::sort(B.begin(), B.end(), std::less<int>());
	print(B);
	// float, lambda
	std::vector<float> AA = { 3.3F, 4.4F, 1.1F, 2.2F, 5.5F, 8.8F, 6.6F, 7.7F, 9.9F, 0.0F };
	std::vector<float> BB(AA);
	print(AA);
	std::sort(BB.begin(), BB.end(), cmp);
	print(BB);
	std::sort(BB.begin(), BB.end(), [](const float a, const float b) { return (a > b); });
	print(BB);
	// done
	return 0;
}
