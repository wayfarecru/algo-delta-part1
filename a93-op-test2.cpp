#include <iostream>
#include <vector>
#include <cassert>
using namespace std;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

void print(const std::vector<int>& v) {
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;
}

int add(const int a, const int b) { // typename: int
	int c;
	c = a + b;
	return c;
}

void add(int n, int c[], const int a[], const int b[]) { // pure C
	for (int i = 0; i < n; ++i) {
		c[i] = a[i] + b[i];
	}
}

// typename: std::vector<int>
std::vector<int> add(const std::vector<int>& a, const std::vector<int>& b) {
	assert(a.size() == b.size());
	std::vector<int> c(a.size());
	for (int i = 0; i < a.size(); ++i) {
		c[i] = a[i] + b[i];
	}
	return c;
}

// typename: std::vector<int>
std::vector<int> operator+(const std::vector<int>& a, const std::vector<int>& b) {
	assert(a.size() == b.size());
	std::vector<int> c(a.size());
	for (int i = 0; i < a.size(); ++i) {
		c[i] = a[i] + b[i];
	}
	return c;
}

int main(void) {
	std::vector<int> A = { 1, 2, 3, 4, 5, 6, 7, 8 };
	std::vector<int> B = { 1, 1, 2, 2, 3, 3, 4, 4 };
	std::vector<int> C;
	// pure C
	C.resize(A.size());
	add(C.size(), C.data(), A.data(), B.data());
	print(C);
	// C++ add()
	C = add(A, B);
	print(C);
	// C++ operator+
	C = A + B;
	print(C);
	// C++ temporary variable
	print(A + B);
	// done
	return 0;
}
