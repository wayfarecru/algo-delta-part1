#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include <queue>
#include "tree.hh"
#include "tree_util.hh"
using namespace std;
using namespace std::chrono;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

int parseNum(int argc, char* argv[], int num = 1000000) {
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
			return val;
		}
	}
	return num;
}

// --------------------------------------------------------------------------------

const int SENTINEL = 0xFFFFFFFF; // sentinel value for empty child

void print_tree(const tree<int>& tr) {
	if (! tr.is_valid(tr.begin())) return;
	int rootdepth = tr.depth(tr.begin());
	std::cout << "-----" << std::endl;
	for (auto it = tr.begin(); it != tr.end(); ++it) {
		for (int i = 0; i < tr.depth(it) - rootdepth; ++i) {
			std::cout << "  ";
		}
		if (*it == SENTINEL) {
			cout << "[empty]" << endl;
		} else {
			std::cout << (*it) << std::endl;
		}
	}
	std::cout << "-----" << std::endl;
}

int main(int argc, char* argv[]) {
	tree<int> tr;
	tree<int>::iterator root = tr.insert(tr.begin(), 68);
	tree<int>::iterator n53 = tr.append_child(root, 53);
	tree<int>::iterator n35 = tr.append_child(root, 35);
	tree<int>::iterator n46 = tr.append_child(n53, 46);
	tree<int>::iterator n39 = tr.append_child(n53, 39);
	tr.append_child(n46, SENTINEL);
	tr.append_child(n46, 31);
	tr.append_child(n39, 20);
	tr.append_child(n39, SENTINEL); // optional
	tr.append_child(n35, SENTINEL);
	tr.append_child(n35, 27);
	// show it
	print_tree(tr);
	// done
	return 0;
}

