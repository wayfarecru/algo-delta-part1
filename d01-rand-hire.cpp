#include <iostream>
#include <random>
#include <algorithm>
using namespace std;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

template<typename T>
void print(const std::vector<T>& v) {
	if (v.size() <= 10) {
		for (const T e : v) {
			cout << e << " ";
		}
	} else { // 0 1 2 3 4 . . . 995 996 997 998 999
		for (int i = 0; i < 5; ++i) {
			cout << v[i] << " ";
		}
		cout << ". . . ";
		for (int i = v.size() - 5; i < v.size(); ++i) {
			cout << v[i] << " ";
		}
	}
	cout << endl;
}

void rand_perm(std::vector<int>& v, std::default_random_engine& re) {
	std::uniform_int_distribution<int> unif_dist(0, v.size() - 1);
	for (int i = 0; i < v.size(); ++i) {
		int j = unif_dist(re);
		std::swap(v[i], v[j]);
	}
}

int hire(std::vector<int> SC, int& nIntv, int& nHire) {
	// at the first interview, you hire the applicant
	int iBest = 0; // index for the current best
	nIntv = 1;
	nHire = 1;
	// main loop
	for (int i = 1; i < SC.size(); ++i) {
		nIntv++;
		if (SC[iBest] < SC[i]) { // better applicant
			iBest = i;
			nHire++;
		}
	}
	return iBest;
}

int main(int argc, char* argv[]) {
	int N = 50;
	if (argc >= 2) { // get the argv[1] to long int
		char* ptr = NULL;
		long val = strtol(argv[1], &ptr, 0);
		if (ptr != NULL && *ptr != '\0') {
			switch (*ptr) {
			case 'k': case 'K': val *= 1024; break; // kilo
			case 'm': case 'M': val *= (1024 * 1024); break; // mega
			case 'g': case 'G': val *= (1024 * 1024 * 1024); break; // giga
			}
		}
		if (val > 0) {
			N = val;
		}
	}
	// worst case: increasing order list
	std::vector<int> SC(N); // score for each applicant
	for (int i = 0; i < SC.size(); ++i) {
		SC[i] = i;
	}
	int nIntv; // number of interviews
	int nHire; // number of hiring
	int iBest = hire(SC, nIntv, nHire);
	cout << "SC[" << SC.size() << "] : ";
	print(SC);
	cout << "worst case:\tSC[" << iBest << "] = " << SC[iBest] << endl;
	cout << "\tnIntv=" << nIntv << ", nHire=" << nHire << endl;
	// randomized hiring with worst case
	std::random_device rd; // random device
	std::default_random_engine re(rd()); // random engine
	rand_perm(SC, re);
	cout << "after rand perm:\tSC[" << SC.size() << "] : ";
	print(SC);
	iBest = hire(SC, nIntv, nHire);
	cout << "rand hire:\tSC[" << iBest << "] = " << SC[iBest] << endl;
	cout << "\tnIntv=" << nIntv << ", nHire=" << nHire << endl;
	cout << "expected nHire = " << log((double)(SC.size())) << endl; // natural log, ln n
	// done
	return 0;
}
