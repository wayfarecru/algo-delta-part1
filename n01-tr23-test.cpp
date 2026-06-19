#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include <deque>
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
using SBTREE = tree<std::deque<T>>; // self-balancing tree

template <typename T>
bool ISNIL(const SBTREE<T>& tr, const typename SBTREE<T>::iterator& it) {
	assert(it != tr.end());
	return (*it).size() == 0; // empty data
}

template <typename T>
const typename SBTREE<T>::iterator PARENT(const SBTREE<T>& tr, const typename SBTREE<T>::iterator& it) {
	try {
		return tr.parent(it);
	} catch (std::exception& e) {
		return tr.end();
	}
}

template <typename T>
typename SBTREE<T>::iterator PARENT(SBTREE<T>& tr, const typename SBTREE<T>::iterator& it) {
	try {
		return tr.parent(it);
	} catch (std::exception& e) {
		return tr.end();
	}
}

template <typename T>
void print_node(SBTREE<T>& tr, typename SBTREE<T>::iterator z) {
	if (z == tr.end()) {
		cout << "tr.end()" << endl;
	} else {
		cout << "SBTREE node data[" << (*z).size() << "] = { ";
		for (int i = 0; i < (*z).size(); ++i) {
			cout << (*z)[i] << " ";
		}
		cout << "}, num child = " << tr.number_of_children(z) << endl;
		for (int i = 0; i < tr.number_of_children(z); ++i) {
			typename SBTREE<T>::iterator y = tr.child(z, i);
			cout << "  child node data[" << (*y).size() << "] = { ";
			for (int i = 0; i < (*y).size(); ++i) {
				cout << (*y)[i] << " ";
			}
			cout << "}" << endl;
		}
	}
}

template <typename T>
void print_rotated(const SBTREE<T>& tr, const typename SBTREE<T>::iterator& it, int depth) {
	if (it == tr.end()) return;
	if (ISNIL(tr, it)) {
		for (int i = 0; i < depth; ++i) {
			cout << "|\t";
		}
		cout << "[NIL]" << endl;
		return;
	}
	int n = (*it).size(); // number of data
	assert(tr.number_of_children(it) == n + 1);
	print_rotated(tr, tr.child(it, n), depth + 1); // rightmost subtree
	for (int i = n - 1; i >= 0; --i) {
		// node
		for (int i = 0; i < depth; ++i) {
			cout << "|\t";
		}
		cout << (*it)[i] << endl;
		// child
		print_rotated(tr, tr.child(it, i), depth + 1);
	}
}

template <typename T>
void print_rotated(const SBTREE<T>& tr) {
	cout << "--------" << endl;
	if (! tr.empty()) {
		print_rotated(tr, tr.begin(), 0);
	}
	cout << "--------" << endl;
}

template <typename T>
typename SBTREE<T>::iterator tr23_find_min(SBTREE<T>& tr, typename SBTREE<T>::iterator it) {
	assert(it != tr.end());
	while ((tr.number_of_children(it) > 0) && (! ISNIL(tr, tr.child(it, 0)))) {
		it = tr.child(it, 0); // move to left-most child
	}
	return it;
}

template <typename T>
typename SBTREE<T>::iterator tr23_find_min(SBTREE<T>& tr) {
	if (tr.empty()) {
		return tr.end();
	} else {
		return tr23_find_min(tr, tr.begin());
	}
}

template <typename T>
typename SBTREE<T>::iterator tr23_find_max(SBTREE<T>& tr, typename SBTREE<T>::iterator it) {
	assert(it != tr.end());
	while ((tr.number_of_children(it) > 0) && (! ISNIL(tr, tr.child(it, tr.number_of_children(it) - 1)))) {
		it = tr.child(it, tr.number_of_children(it) - 1); // move to right-most child
	}
	return it;
}

template <typename T>
typename SBTREE<T>::iterator tr23_find_max(SBTREE<T>& tr) {
	if (tr.empty()) {
		return tr.end();
	} else {
		return tr23_find_max(tr, tr.begin());
	}
}

template <typename T>
bool tr23_validate(SBTREE<T>& tr, typename SBTREE<T>::iterator it) {
	assert(it != tr.end());
	int n = (*it).size(); // number of data
	if (n == 0) { // NIL node
		if (tr.number_of_children(it) != 0) {
			cout << "a NIL node has error: number of children = " << tr.number_of_children(it) << endl;
			return false;
		}
		return true;
	}
	if (n != 1 && n != 2) {
		cout << "a node has error: number of data = " << n << " is not 1 or 2" << endl;
		return false;
	}
	if (tr.number_of_children(it) != n + 1) {
		cout << "a node has error: number of data = " << n
		     << ", while number of children = " << tr.number_of_children(it) << endl;
		return false;
	}
	for (int i = 0; i < n; ++i) {
		typename SBTREE<T>::iterator z = tr23_find_max(tr, tr.child(it, i));
		assert(z != tr.end());
		if (((*z).size() > 0) && ((*z).back() > (*it)[i])) {
			cout << "a node with value=" << (*it)[i] << " has left subtree whose max=" << (*z)[0] << endl;
			return false;
		}
	}
	typename SBTREE<T>::iterator z = tr23_find_min(tr, tr.child(it, n));
	if (((*z).size() > 0) && ((*z).front() < (*it).back())) {
		cout << "a node with value=" << (*it).back() << " has right subtree whose min=" << (*z).front() << endl;
		return false;
	}
	for (int i = 0; i < n; ++i) {
		if (tr23_validate(tr, tr.child(it, i)) == false) {
			return false;
		}
	}
	return true;
}

template <typename T>
bool tr23_validate(SBTREE<T>& tr) {
	if (tr.empty()) {
		return true;
	} else {
		return tr23_validate(tr, tr.begin());
	}
}

template <typename T>
std::pair<typename SBTREE<T>::iterator, int> tr23_find(SBTREE<T>& tr, typename SBTREE<T>::iterator it, const T& key) {
	while (! ISNIL(tr, it)) {
		int n = (*it).size(); // number of data
		assert(n == 1 || n == 2); // this is 2-3 tree
		assert(tr.number_of_children(it) == n + 1);
		bool found = false;
		for (int i = 0; i < n; ++i) {
			if (key == (*it)[i]) {
				return make_pair(it, i); // FOUND
			} else if (key < (*it)[i]) {
				it = tr.child(it, i);
				found = true;
				break;
			}
		}
		if (found == false) {
			it = tr.child(it, n); // right-most child
		}
	}
	return make_pair(tr.end(), 0); // NOT FOUND
}

template <typename T>
std::pair<typename SBTREE<T>::iterator, int> tr23_find(SBTREE<T>& tr, const T& key) {
	if (tr.empty()) {
		return make_pair(tr.end(), 0); // NOT FOUND
	} else {
		return tr23_find(tr, tr.begin(), key);
	}
}

template <typename T>
void tr23_fixup_after_insert(SBTREE<T>& tr, typename SBTREE<T>::iterator z) {
	typename SBTREE<T>::iterator p = PARENT(tr, z);
	while (p != tr.end()) {
		assert((*z).size() == 1);
		assert(tr.number_of_children(z) == 2);
		int n = (*p).size(); // number of data
		assert(n == 1 || n == 2); // this is 2-3 tree
		assert(tr.number_of_children(p) == n + 1);
		if (n == 2) {
			if (tr.index(z) == 0) { // case 3A: z is the left child of p
				typename SBTREE<T>::iterator y = tr.append_child(p, std::deque<T>());
				// arrange data
				(*y).push_back((*p)[1]);
				(*p).pop_back();
				// arrange subtrees
				typename SBTREE<T>::iterator w = tr.append_child(y, std::deque<T>()); // dummy child
				typename SBTREE<T>::iterator c = tr.child(p, 1);
				typename SBTREE<T>::iterator d = tr.child(p, 2);
				tr.move_after(w, c);
				tr.move_after(w, d);
				tr.erase(w);
				assert(tr.number_of_children(p) == 2);
				assert(tr.number_of_children(z) == 2);
				assert(tr.number_of_children(y) == 2);
			} else if (tr.index(z) == 1) { // case 3B: z is the middle child of p
				typename SBTREE<T>::iterator y = tr.append_child(p, std::deque<T>());
				// arrange data 
				(*y).push_back((*p)[1]);
				(*p).pop_back();
				(*z).push_back((*p)[0]);
				(*p).push_back((*z)[0]);
				(*p).pop_front();
				(*z).pop_front();
				// arrange subtrees
				typename SBTREE<T>::iterator w = tr.append_child(y, std::deque<T>()); // dummy child
				typename SBTREE<T>::iterator a = tr.child(p, 0);
				typename SBTREE<T>::iterator b = tr.child(z, 0);
				typename SBTREE<T>::iterator c = tr.child(z, 1);
				typename SBTREE<T>::iterator d = tr.child(p, 2);
				tr.move_after(w, c);
				tr.move_after(c, d);
				tr.move_before(b, a);
				tr.erase(w);
				assert(tr.number_of_children(p) == 2);
				assert(tr.number_of_children(z) == 2);
				assert(tr.number_of_children(y) == 2);
			} else { // case 3C: z is the right child of p
				assert(tr.index(z) == 2);
				typename SBTREE<T>::iterator x = tr.prepend_child(p, std::deque<T>());
				// arrange data
				(*x).push_back((*p)[0]);
				(*p).pop_front();
				// arrange subtrees
				typename SBTREE<T>::iterator w = tr.append_child(x, std::deque<T>()); // dummy child
				typename SBTREE<T>::iterator a = tr.child(p, 1);
				typename SBTREE<T>::iterator b = tr.child(p, 2);
				tr.move_after(w, a);
				tr.move_after(a, b);
				tr.erase(w);
				assert(tr.number_of_children(p) == 2);
				assert(tr.number_of_children(z) == 2);
				assert(tr.number_of_children(x) == 2);
			}
			z = p;
			p = PARENT(tr, z);
		} else {
			assert(n == 1);
			if (z == tr.child(p, 0)) { // case 2A: z is the left child of p
				assert((*z)[0] < (*p)[0]);
				typename SBTREE<T>::iterator l = tr.child(z, 0);
				typename SBTREE<T>::iterator m = tr.child(z, 1);
				typename SBTREE<T>::iterator r = tr.child(p, 1);
				(*p).push_front((*z)[0]);
				tr.move_before(r, m);
				tr.move_before(m, l);
				tr.erase(z);
			} else { // case 2B
				assert(z == tr.child(p, 1));
				assert((*p)[0] < (*z)[0]);
				typename SBTREE<T>::iterator l = tr.child(p, 0);
				typename SBTREE<T>::iterator m = tr.child(z, 0);
				typename SBTREE<T>::iterator r = tr.child(z, 1);
				(*p).push_back((*z)[0]);
				tr.move_after(l, m);
				tr.move_after(m, r);
				tr.erase(z);
			}
			return; // done
		}
	}
}

template <typename T>
void tr23_insert(SBTREE<T>& tr, typename SBTREE<T>::iterator it, const T& key) {
	// move downward to a NIL node
	while (! ISNIL(tr, it)) {
		int n = (*it).size(); // number of data
		assert(n == 1 || n == 2); // this is 2-3 tree
		assert(tr.number_of_children(it) == n + 1);
		bool found = false;
		for (int i = 0; i < n; ++i) {
			if (key == (*it)[i]) {
				return; // FOUND
			} else if (key < (*it)[i]) {
				it = tr.child(it, i);
				found = true;
				break;
			}
		}
		if (found == false) {
			it = tr.child(it, n); // right-most child
		}
	}
	// insert data into the NIL node
	(*it).push_back(key);
	tr.append_child(it, std::deque<T>(0)); // null data for left child
	tr.append_child(it, std::deque<T>(0)); // null data for right child
	// move upward to re-arrange the 2-3 tree
	tr23_fixup_after_insert(tr, it);
}

template <typename T>
void tr23_insert(SBTREE<T>& tr, const T& key) {
	if (tr.empty()) {
		tr.insert(tr.begin(), std::deque<T>(0)); // null data
	}
	tr23_insert(tr, tr.begin(), key);
}

int main(int argc, char* argv[]) {
	std::string s = "ALGORITHMS";
	std::vector<char> v(s.begin(), s.end());
	SBTREE<char> tr;
	for (char& c : v) {
		tr23_insert(tr, c);
	}
	// show it
	print_rotated(tr);
	cout << "min = " << (*tr23_find_min(tr)).front() << endl;
	cout << "max = " << (*tr23_find_max(tr)).back() << endl;
	cout << "validate = " << std::boolalpha << tr23_validate(tr) << endl;
	// find elements
	for (char& c : v) {
		std::pair<SBTREE<char>::iterator, int> p = tr23_find(tr, c);
		if (p.first != tr.end()) {
			cout << (*(p.first))[p.second] << endl;
		} else {
			cout << "element = " << c << " : NOT FOUND" << endl;
		}
	}
	// done
	return 0;
}
