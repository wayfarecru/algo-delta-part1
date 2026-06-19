#include <iostream>
#include <vector>
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

template<typename T>
struct Less {
	bool operator()(const T& a, const T& b) const { return (a < b); }
};

template<typename T>
struct Greater {
	bool operator()(const T& a, const T& b) const { return (a > b); }
};

template<typename T>
bool GreaterFunc(const T& a, const T& b) {
	return (a > b);
}

template<typename T, typename Compare = std::less<T>>
void ins_sort(const std::vector<T>& A, std::vector<T>& B, const Compare& compare = Compare()) {
	B.resize(A.size());
	for (int i = 0; i < A.size(); ++i) {
		T key = A[i];
		// insert A[i] into the sorted sub-array B[0:i-1]
		int j = i - 1;
		while ((j >= 0) && compare(key, B[j])) {
			B[j + 1] = B[j];
			--j;
		}
		B[j + 1] = key;
	}
}

int main(void) {
	// int
	std::vector<int> A = { 3, 4, 1, 2, 5, 8, 6, 7, 9, 0 };
	std::vector<int> B;
	print(A);
	ins_sort(A, B);
	print(B);
	// int, reverse order, function pointer
	ins_sort(A, B, GreaterFunc<int>);
	print(B);
	// float, functor
	std::vector<float> AA = { 3.3F, 4.4F, 1.1F, 2.2F, 5.5F, 8.8F, 6.6F, 7.7F, 9.9F, 0.0F };
	std::vector<float> BB;
	print(AA);
	ins_sort(AA, BB, Greater<float>());
	print(BB);
	// done
	return 0;
}
