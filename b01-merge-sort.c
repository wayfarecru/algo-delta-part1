#include <stdio.h>
#include <assert.h>
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

void merge(int n, int A[n], int B[n], int p, int q, int r) {
	assert(A != B);
	// copy A[p..r) to B[p..r)
	for (int i = p; i < r; ++i) {
		B[i] = A[i];
	}
	// merge B[p..q) and B[q..r) to A[p..r)
	int i = p; // index for B[p..q)
	int j = q; // index for B[q..r)
	int k = p; // index to A[p..r)
	while ((i < q) && (j < r)) {
		if (B[i] <= B[j]) {
			A[k++] = B[i++];
		} else {
			A[k++] = B[j++];
		}
	}
	while (i < q) {
		A[k++] = B[i++];
	}
	while (j < r) {
		A[k++] = B[j++];
	}
}

void merge_sort(int n, int A[n], int B[n], int p, int r) {
	assert(A != B);
	if ((r - p) <= 1) return; // if 0 or 1 element, just return
	int q = (p + r) / 2; // mid point
	merge_sort(n, A, B, p, q); // sort A[p..q)
	merge_sort(n, A, B, q, r); // sort A[q..r)
	merge(n, A, B, p, q, r);
}

int main(void) {
	enum { N = 16 };
	int A[N] = { 3, 4, 1, 2, 5, 8, 6, 7, 9, 0, 10, 15, 14, 12, 13, 11 };
	int B[N]; // buffer for merge
	printf("before:\t\tA[%d] : ", N);
	print(N, A);
	merge_sort(N, A, B, 0, N);
	printf("merge_sort:\tA[%d] : ", N);
	print(N, A);
	return 0;
}
