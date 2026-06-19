#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include <queue>
#include <exception>
#include <limits>
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

std::random_device rd; // random device
std::default_random_engine re(rd()); // random engine

template<typename T>
void setRand(typename std::vector<T>::iterator first, typename std::vector<T>::iterator last) {
	if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
		std::uniform_real_distribution<T> unif_dist(0.0F, 1.0F);
		for (typename std::vector<T>::iterator it = first; it != last; ++it) {
			*it = unif_dist(re);
		}
	} else {
		std::uniform_int_distribution<T> unif_dist(0, 9999999);
		for (typename std::vector<T>::iterator it = first; it != last; ++it) {
			*it = unif_dist(re);
		}
	}
}

template<typename T>
void setRand(std::vector<T>& v) {
	setRand<T>(v.begin(), v.end());
}

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

template <typename T>
const T SENTINEL(void) {
	if constexpr (std::is_same_v<T, std::string>) {
		return static_cast<T>(nullptr);
	} else {
		return std::numeric_limits<T>::max();
	}
}

template <typename T>
const typename tree<T>::iterator PARENT(const tree<T>& tr, const typename tree<T>::iterator& it) {
	try {
		return tr.parent(it);
	} catch (std::exception& e) {
		return tr.end();
	}
}

template <typename T>
typename tree<T>::iterator PARENT(tree<T>& tr, const typename tree<T>::iterator& it) {
	try {
		return tr.parent(it);
	} catch (std::exception& e) {
		return tr.end();
	}
}

template <typename T>
void print_rotated(const tree<T>& tr, const typename tree<T>::iterator& it, int depth) {
	int n = tr.number_of_children(it);
	// right sub-tree
	if (n >= 2) {
		for (int i = n - 1; i >= 1; --i) {
			print_rotated(tr, tr.child(it, i), depth + 1);
		}
	} else if (n == 1) { // one blank line for empty right child
		for (int i = 0; i < depth + 2; ++i) {
			cout << "|\t";
		}
		cout << endl;
	}
	// node it self
	for (int i = 0; i < depth; ++i) {
		cout << "|\t";
	}
	if (*it == SENTINEL<T>()) {
		cout << "[empty]" << endl;
	} else {
		cout << *it << endl;
	}
	// left sub-tree
	if (n > 0) {
		print_rotated(tr, tr.child(it, 0), depth + 1);
	}
}

template <typename T>
void print_rotated(const tree<T>& tr) {
	cout << "--------" << endl;
	if (! tr.empty()) {
		print_rotated(tr, tr.begin(), 0);
	}
	cout << "--------" << endl;
}

template <typename T>
typename tree<T>::iterator bst_insert(tree<T>& tr, typename tree<T>::iterator it, T key) {
	while (*it != key) {
		if (key < *it) { // case B: move to left subtree
			if (tr.number_of_children(it) == 0) { // case B1: no child
				return tr.append_child(it, key);
			}
			typename tree<T>::iterator cit = tr.child(it, 0);
			if (*cit == SENTINEL<T>()) { // case B3: only right child
				*cit = key;
				return cit;
			}
			it = cit; // case B2, B4: left child only or two children
		} else { // case C: move to right subtree
			int n = tr.number_of_children(it);
			if (n == 0) { // case C1: no child
				tr.append_child(it, SENTINEL<T>()); // left child
				return tr.append_child(it, key); // right child
			}
			if (n == 1) { // case C2: left child only
				return tr.append_child(it, key); // right child
			}
			it = tr.child(it, 1); // case C3, C4: right child only or two children
		}
	}
	// case A: we found the value node
	*it = key;
	return it;
}

template <typename T>
typename tree<T>::iterator bst_insert(tree<T>& tr, T key) {
	if (tr.empty()) {
		return tr.insert(tr.begin(), key);
	} else {
		return bst_insert(tr, tr.begin(), key);
	}
}

template <typename T>
typename tree<T>::iterator bst_find(const tree<T>& tr, typename tree<T>::iterator it, T key) {
	while (*it != key) {
		if (key < *it) { // case B: move to left subtree
			if (tr.number_of_children(it) == 0) { // case B1: no child
				return tr.end();
			}
			typename tree<T>::iterator cit = tr.child(it, 0);
			if (*cit == SENTINEL<T>()) { // case B3: only right child
				return tr.end();
			}
			it = cit; // case B2, B4: left child only or two children
		} else { // case C: move to right subtree
			int n = tr.number_of_children(it);
			if (n <= 1) { // case C1, C2: no child or left child only
				return tr.end();
			}
			it = tr.child(it, 1); // case C3, C4: right child only or two children
		}
	}
	// case A: we found the value node
	return it;
}

template <typename T>
typename tree<T>::iterator bst_find(const tree<T>& tr, T key) {
	if (tr.empty()) {
		return tr.end();
	} else {
		return bst_find(tr, tr.begin(), key);
	}
}

template <typename T>
typename tree<T>::iterator bst_find_min(const tree<T>& tr, typename tree<T>::iterator it) {
	assert(it != tr.end());
	while ((tr.number_of_children(it) != 0) && (*tr.child(it, 0) != SENTINEL<T>())) {
		it = tr.child(it, 0); // move to left child
	}
	return it;
}

template <typename T>
typename tree<T>::iterator bst_find_min(const tree<T>& tr) {
	if (tr.empty()) {
		return tr.end();
	} else {
		return bst_find_min(tr, tr.begin());
	}
}

template <typename T>
typename tree<T>::iterator bst_find_max(const tree<T>& tr, typename tree<T>::iterator it) {
	assert(it != tr.end());
	while (tr.number_of_children(it) == 2) {
		it = tr.child(it, 1); // move to right child
	}
	return it;
}

template <typename T>
typename tree<T>::iterator bst_find_max(const tree<T>& tr) {
	if (tr.empty()) {
		return tr.end();
	} else {
		return bst_find_max(tr, tr.begin());
	}
}

template <typename T>
typename tree<T>::iterator bst_find_succ(const tree<T>& tr, typename tree<T>::iterator it) { // successor
	if (tr.empty() || it == tr.end()) {
		return tr.end();
	}
	if (tr.number_of_children(it) == 2) { // if it has right sub-tree
		return bst_find_min(tr, tr.child(it, 1));
	} else { // no right sub-tree: find the lowest ancestor
		typename tree<T>::iterator pit = PARENT(tr, it);
		while ((pit != tr.end()) && (tr.number_of_children(pit) == 2) && (tr.child(pit, 1) == it)) {
			it = pit;
			pit = PARENT(tr, it);
		}
		return pit;
	}
}

template <typename T>
typename tree<T>::iterator bst_find_pred(const tree<T>& tr, typename tree<T>::iterator it) { // predecessor
	if (tr.empty() || it == tr.end()) {
		return tr.end();
	}
	if ((tr.number_of_children(it) > 0) && (*tr.child(it, 0) != SENTINEL<T>())) { // if it has left sub-tree
		return bst_find_max(tr, tr.child(it, 0));
	} else { // no left sub-tree: find the lowest ancestor
		typename tree<T>::iterator pit = PARENT(tr, it);
		while ((pit != tr.end()) && (tr.number_of_children(pit) > 0) && (tr.child(pit, 0) == it)) {
			it = pit;
			pit = PARENT(tr, it);
		}
		return pit;
	}
}

template <typename T>
void bst_erase(tree<T>& tr, typename tree<T>::iterator z) {
	assert(tr.empty() == false);
	assert(z != tr.end());
	int num_child = tr.number_of_children(z);
	// case 0: z has no child
	if (num_child == 0) {
		if (tr.index(z) == 0) {	// z is left child of p
			if (tr.number_of_siblings(z) == 0) { // case 01: z is the single left child, so just remove it
				tr.erase(z);
			} else { // case 02: p has right child, so make z sentinel
				assert(tr.number_of_siblings(z) == 1);
				*z = SENTINEL<T>();
			}
		} else { // z is right child of its parent
			assert(tr.index(z) == 1);
			assert(tr.number_of_siblings(z) == 1);
			typename tree<T>::iterator l = tr.previous_sibling(z); // left child of p
			if (*l == SENTINEL<T>()) { // case 03: l is sentinel, remove it
				assert(tr.number_of_children(l) == 0);
				tr.erase(l);
			}
			// case 03, 04: z is right child of p
			tr.erase(z);
		}
		return; // done for case 0
	}
	// case B: z has only left subtree
	typename tree<T>::iterator l = tr.child(z, 0); // left child of z
	if (num_child == 1) {
		tr.move_after(z, l);
		tr.erase(z);
		assert(tr.number_of_siblings(l) <= 1); // 0 or 1 siblings for l
		return;
	}
	assert(num_child == 2);
	typename tree<T>::iterator r = tr.child(z, 1); // right child of z
	// case A: z has only right subtree
	if (*l == SENTINEL<T>()) {
		assert(tr.number_of_children(l) == 0);
		tr.move_after(z, r);
		tr.erase(l);
		tr.erase(z);
		assert(tr.number_of_siblings(r) <= 1); // 0 or 1 siblings for r
		return;
	}
	// case C: z has two children, and its successor y is just its right child r
	typename tree<T>::iterator y = bst_find_succ(tr, z); // successor
	typename tree<T>::iterator d; // dummy child
	if (y == r) {
		if (tr.number_of_children(r) == 0) {
			d = tr.append_child(r, SENTINEL<T>()); // dummy child inserted to right child
		} else {
			assert(tr.number_of_children(r) == 2);
			d = tr.child(r, 0); // sentinel left child of right child
		}
		tr.move_before(d, l); // l moved to left child of y
		tr.erase(d);
		tr.move_after(z, y);
		tr.erase(z);
		assert(tr.number_of_siblings(y) <= 1);
		assert(tr.number_of_children(y) > 0);
		return;
	}
	// case D: z has two children, and its successor y is far away from its right child r
	typename tree<T>::iterator w = PARENT(tr, y); // parent of y
	assert(w != tr.end());
	if (tr.number_of_children(y) == 0) {
		// case D1: y has no child, w=parent(y) has only left child y
		// case D2: y has no child, w has two children
		d = tr.append_child(y, SENTINEL<T>()); // dummy child inserted to successor
		tr.move_before(d, l); // l moved to left child of its successor
		if (tr.number_of_children(w) == 1) {
			tr.erase(d);
		} else {
			tr.move_after(y, d);
		}
	} else {
		assert(tr.number_of_children(y) == 2);
		// case D3: y has right child, w=parent(y) has only left child y
		// case D4: y has right child, w has two children
		d = tr.child(y, 0); // left child of y is sentinel
		assert(*d == SENTINEL<T>());
		assert(tr.number_of_children(d) == 0);
		typename tree<T>::iterator x = tr.child(y, 1); // x is right child of y
		tr.move_after(y, x);
		tr.move_before(d, l); // lit moved to left child of its successor
		tr.erase(d);
	}
	tr.move_after(z, y);
	tr.move_after(l, r);
	tr.erase(z);
	assert(tr.number_of_siblings(y) <= 1);
	assert(tr.number_of_children(y) == 2);
}

template <typename T>
void bst_erase(tree<T>& tr, T key) {
	typename tree<T>::iterator z = bst_find(tr, key);
	if (z != tr.end()) {
		bst_erase(tr, z);
	}
}

template <typename T>
bool bst_validate(const tree<T>& tr, const typename tree<T>::iterator& it) {
	assert(it != tr.end());
	if (*it == SENTINEL<T>()) {
		if (tr.number_of_children(it) != 0) {
			cout << "VALIDATE: sentinel node=" << *it << " has " << tr.number_of_children(it) << " child(ren)" << endl;
			cout << "VALIDATE: parent = " << *PARENT(tr, it) << endl;
			cout << "VALIDATE: child = ";
			for (int i = 0; i < tr.number_of_children(it); ++i) {
				cout << *tr.child(it, i) << " ";
			}
			cout << endl;
			return false;
		}
		return true;
	}
	if (tr.number_of_children(it) > 2) {
		cout << "VALIDATE: node=" << *it << " has " << tr.number_of_children(it) << " child(ren)" << endl;
		return false;
	}
	if (tr.number_of_children(it) == 1) { // only left child
		typename tree<T>::iterator lit = tr.child(it, 0); // left child
		if (*lit == SENTINEL<T>()) {
			cout << "VALIDATE: dangling sentinel found as left child of value=" << *it << endl;
			return false;
		}
		if (*lit > *it) {
			cout << "VALIDATE: left-child=" << *lit << ", node=" << *it << endl;
			return false;
		}
		return bst_validate(tr, lit);
	} else if (tr.number_of_children(it) == 2) {
		typename tree<T>::iterator rit = tr.child(it, 1); // right child
		if (*rit == SENTINEL<T>()) {
			cout << "VALIDATE: illegal sentinel found as right child of value=" << *it << endl;
			return false;
		}
		if (*it > *rit) {
			cout << "VALIDATE: node=" << *it << ", right-child=" << *rit << endl;
			return false;
		}
		if (bst_validate(tr, rit) != true) return false;
		typename tree<T>::iterator lit = tr.child(it, 0); // left child
		if ((*lit != SENTINEL<T>()) && (*lit > *it)) {
			cout << "VALIDATE: left-child=" << *lit << ", node=" << *it << endl;
			return false;
		}
		return bst_validate(tr, lit);
	}
	return true;
}

template <typename T>
bool bst_validate(const tree<T>& tr) {
	if (tr.empty()) {
		return true;
	} else {
		return bst_validate(tr, tr.begin());
	}
}

int main(int argc, char* argv[]) {
	int N = parseNum(argc, argv, 32);
	std::vector<int> A(N);
	tree<int> tr;
	std::set<int> S;
	setRand(A);
	// insert to my tree implementation
	cout << "my BST insert ------" << endl;
	{
		steady_clock::time_point start = steady_clock::now();
		for (auto& v : A) {
			bst_insert(tr, v);
		}
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	if (N <= 32) {
		print_rotated(tr);
	}
	cout << "num elem = " << tr.size() << endl;
	cout << "max depth = " << tr.max_depth() << endl;
	cout << "min = " << *bst_find_min(tr) << endl;
	cout << "max = " << *bst_find_max(tr) << endl;
	cout << "validate = " << std::boolalpha << bst_validate(tr) << endl;
	// insert to std::set
	cout << "std::set insert ------" << endl;
	{
		steady_clock::time_point start = steady_clock::now();
		for (auto& v : A) {
			S.insert(v);
		}
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "size = " << S.size() << endl;
	cout << "min = " << *std::min_element(S.begin(), S.end()) << endl;
	cout << "max = " << *std::max_element(S.begin(), S.end()) << endl;
	// erase from my tree implementation
	cout << "my BST erase ------" << endl;
	{
		steady_clock::time_point start = steady_clock::now();
		for (auto& v : A) {
			bst_erase(tr, v);
		}
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	cout << "after all erase ------" << endl;
	cout << "validate = " << std::boolalpha << bst_validate(tr) << endl;
	// erase from std::set
	cout << "std::set erase ------" << endl;
	{
		steady_clock::time_point start = steady_clock::now();
		for (auto& v : A) {
			S.erase(v);
		}
		cout << "\t" << duration_cast<milliseconds>(steady_clock::now() - start).count() << " msec elapsed" << endl;
	}
	// done
	return 0;
}
