#include <stdio.h>
#include <stdlib.h> // for strtol()
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

int main(int argc, char* argv[]) {
	// print out all argv[]'s
	printf("argc = %d\n", argc);
	for (int i = 0; i < argc; ++i) {
		printf("argv[%d] = \"%s\"\n", i, argv[i]);
	}
	// convert argv[1] to long int
	if (argc >= 2) {
		char* ptr = NULL;
		long val = strtol(argv[1], &ptr, 0);
		if (ptr != NULL && *ptr != '\0') {
			switch (*ptr) {
			case 'k': case 'K': val *= 1024; break; // kilo
			case 'm': case 'M': val *= (1024 * 1024); break; // mega
			case 'g': case 'G': val *= (1024 * 1024 * 1024); break; // giga
			}
		}
		printf("val = %ld\n", val);
	}
	return 0;
}
