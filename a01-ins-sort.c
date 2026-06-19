#ifdef __cplusplus
#error This file works only with pure C
#endif

#include <stdio.h>
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

void print(int n, int A[n]) {
	for (int i = 0; i < n; ++i) {
		printf("%d ", A[i]);
	}
	printf("\n");
}

void ins_sort(int n, const int A[n], int B[n]) { // insertion sort
	for (int i = 0; i < n; ++i) {
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
	enum { N = 10 }; // pure C-style constant
	int A[N] = { 3, 4, 1, 2, 5, 8, 6, 7, 9, 0 };
	int B[N];
	printf("A[%d] : ", N);
	print(N, A);
	ins_sort(N, A, B);
	printf("B[%d] : ", N);
	print(N, B);
	return 0;
}
