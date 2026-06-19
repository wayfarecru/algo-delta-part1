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
void ins_sort(const std::vector<T>& A, std::vector<T>& B) {
	B.resize(A.size());
	for (int i = 0; i < A.size(); ++i) {
		T key = A[i];
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
	// int
	std::vector<int> A = { 3, 4, 1, 2, 5, 8, 6, 7, 9, 0 };
	std::vector<int> B;
	cout << "A[" << A.size() << "] : ";
	print(A);
	ins_sort(A, B);
	cout << "B[" << B.size() << "] : ";
	print(B);
	// float
	std::vector<float> AA = { 3.3F, 4.4F, 1.1F, 2.2F, 5.5F, 8.8F, 6.6F, 7.7F, 9.9F, 0.0F };
	std::vector<float> BB;
	cout << "AA[" << AA.size() << "] : ";
	print(AA);
	ins_sort(AA, BB);
	cout << "BB[" << BB.size() << "] : ";
	print(BB);
	// done
	return 0;
}
