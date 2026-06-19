#ifndef __cplusplus
#error This file works only with C++
#endif

#include <iostream>
#include <vector>
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

void ins_sort(const std::vector<int>& A, std::vector<int>& B) {
	B.resize(A.size());
	for (int i = 0; i < A.size(); ++i) {
		int key = A[i];
		// insert A[i] into the sorted sub-array B[0:i-1]
		int j = i - 1;
		while ((j >= 0) && (key < B[j])) {
			B[j + 1] = B[j];
			--j;
		}
		B[j + 1] = key;
	}
}

int main(void) {
	std::vector<int> A = { 3, 4, 1, 2, 5, 8, 6, 7, 9, 0 };
	std::vector<int> B;
	cout << "A[" << A.size() << "] : ";
	print(A);
	ins_sort(A, B);
	cout << "B[" << B.size() << "] : ";
	print(B);
	return 0;
}
