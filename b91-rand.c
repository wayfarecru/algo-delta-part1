#include <stdio.h>
#include <stdlib.h> // for rand()
#include <time.h> // for time()
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

int main(void) {
	printf("RAND_MAX = %d\n", RAND_MAX);
	// seed 0 test
	srand(0);
	printf("random numbers, with seed 0\n");
	for (int i = 0; i < 10; ++i) {
		printf("%d\n", rand());
	}
	// histogram test
	srand(time(NULL)); // different seed for each execution
	enum { N = 1000000, M = 20 };
	printf("histogram test with %d trials into A[%d]\n", N, M);
	int A[M] = { 0 };
	for (int i = 0; i < N; ++i) {
		int ind = (rand() % M);
		A[ind]++;
	}
	for (int i = 0; i < M; ++i) {
		printf("A[%2d] = %8d\n", i, A[i]);
	}
}
