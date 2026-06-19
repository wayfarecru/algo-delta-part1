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

void print_tree(const tree<int>& tr) {
	if (! tr.is_valid(tr.begin())) return;
	int rootdepth = tr.depth(tr.begin());
	std::cout << "-----" << std::endl;
	for (auto it = tr.begin(); it != tr.end(); ++it) {
		for (int i = 0; i < tr.depth(it) - rootdepth; ++i) {
			std::cout << "  ";
		}
		std::cout << (*it) << std::endl;
	}
	std::cout << "-----" << std::endl;
}

template<typename T>
void gen_bin_tree(tree<T>& tr, vector<typename tree<T>::iterator>& ptr, int N) {
	tr.clear();
	ptr.clear();
	ptr.resize(N);
	ptr[0] = tr.insert(tr.begin(), static_cast<T>(1)); // root note
	for (int i = 1; i < N; ++i) {
		int parent_ind = (i - 1) / 2;
		auto qtr = ptr[parent_ind];
		ptr[i] = tr.append_child(qtr, static_cast<T>(i + 1));
	}
}

template<typename T>
void breadth_first_search(const tree<T>& tr) {
	queue<typename tree<T>::iterator> q;
	// insert root node to the queue
	q.push(tr.begin());
	while (! q.empty()) {
		// print the node
		typename tree<T>::iterator p = q.front();
		cout << *p << " ";
		q.pop();
		// push all children
		for (int i = 0; i < tr.number_of_children(p); ++i) {
			q.push(tr.child(p, i));
		}
	}
	cout << endl;
}

template<typename T>
void depth_first_search(const tree<T>& tr, const typename tree<T>::iterator p) {
	cout << *p << " ";
	// push all children
	for (int i = 0; i < tr.number_of_children(p); ++i) {
		depth_first_search(tr, tr.child(p, i));
	}
}

template<typename T>
void depth_first_search(const tree<T>& tr) {
	depth_first_search<T>(tr, tr.begin());
	cout << endl;
}

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 27);
	tree<int> tr;
	vector<tree<int>::iterator> ptr;
	// generate full binary tree
	gen_bin_tree(tr, ptr, N);
	print_tree(tr);
	// breadth first search
	breadth_first_search(tr);
	// depth first search
	depth_first_search(tr);
	// done
	return 0;
}
