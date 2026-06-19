#include <stdio.h>
#include <stdlib.h> // for rand()
#include <time.h> // for time()
#include <string.h> // for memcpy()
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

void print(int n, int A[n]) {
	if (n <= 10) {
		for (int i = 0; i < n; ++i) {
			printf("%d ", A[i]);
		}
	} else { // 0 1 2 3 4 . . . 995 996 997 998 999
		for (int i = 0; i < 5; ++i) {
			printf("%d ", A[i]);
		}
		printf(". . . ");
		for (int i = n - 5; i < n; ++i) {
			printf("%d ", A[i]);
		}
	}
	printf("\n");
}

void merge(int n, int A[n], int B[n], int p, int q, int r) {
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
	if ((r - p) <= 1) return; // if 0 or 1 element, just return
	int q = (p + r) / 2; // mid point
	merge_sort(n, A, B, p, q); // sort A[p..q)
	merge_sort(n, A, B, q, r); // sort A[q..r)
	merge(n, A, B, p, q, r);
}

void setRand(int n, int A[n]) {
	for (int i = 0; i < n; ++i) {
		A[i] = (rand() % 1000); // data will be ranged 0 to 1000
	}
}

int compare(const void* a, const void* b) {
	const int* aptr = (const int*)(a);
	const int* bptr = (const int*)(b);
	return (*aptr - *bptr);
}

void diff(int n, int A[n], int B[n]) {
	for (int i = 0; i < n; ++i) {
		if (A[i] != B[i]) {
			printf("A[%d] = %d, B[%d] = %d : different ********\n", i, A[i], i, B[i]);
			return;
		}
	}
	printf("identical\n");
}

int main(void) {
	enum { N = 10000 };
	int A[N];
	int B[N]; // buffer for merge
	int C[N];
	srand(time(NULL));
	setRand(N, A);
	memcpy(C, A, sizeof(A));
	// merge_sort
	printf("before:\t\tA[%d] : ", N);
	print(N, A);
	merge_sort(N, A, B, 0, N);
	printf("merge_sort:\tA[%d] : ", N);
	print(N, A);
	// qsort
	printf("before:\t\tC[%d] : ", N);
	print(N, C);
	qsort(C, N, sizeof(int), compare);
	printf("qsort:\t\tC[%d] : ", N);
	print(N, C);
	// diff
	diff(N, A, C);
	// done
	return 0;
}
