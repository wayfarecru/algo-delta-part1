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

enum RBFLAG { BLACK = 0, RED = 1, DUMMY = 2 }; // red-black tree flag

template <typename T>
using RBTREE = tree<std::pair<RBFLAG, T>>;

template <typename T>
const typename RBTREE<T>::iterator PARENT(const RBTREE<T>& tr, const typename RBTREE<T>::iterator& it) {
	try {
		return tr.parent(it);
	} catch (std::exception& e) {
		return tr.end();
	}
}

template <typename T>
typename RBTREE<T>::iterator PARENT(RBTREE<T>& tr, const typename RBTREE<T>::iterator& it) {
	try {
		return tr.parent(it);
	} catch (std::exception& e) {
		return tr.end();
	}
}

template <typename T>
void rb_print(const RBTREE<T>& tr, const typename RBTREE<T>::iterator& it, int depth) {
	int n = tr.number_of_children(it);
	// right sub-tree
	if (n >= 2) {
		for (int i = n - 1; i >= 1; --i) {
			rb_print(tr, tr.child(it, i), depth + 1);
		}
	} else if (n == 1) { // one blank line for empty right child
		for (int i = 0; i < depth + 2; ++i) {
			cout << "|\t";
		}
		cout << endl;
	}
	// node itself
	for (int i = 0; i < depth; ++i) {
		cout << "|\t";
	}
	switch ((*it).first) {
	case RBFLAG::BLACK:
		cout << (*it).second << endl;
		break;
	case RBFLAG::RED:
		cout << (*it).second << " (RED)" << endl;
		break;
	case RBFLAG::DUMMY:
		cout << "[empty]" << endl;
		break;
	}
	// left sub-tree
	if (n > 0) {
		rb_print(tr, tr.child(it, 0), depth + 1);
	}
}

template <typename T>
void rb_print(const RBTREE<T>& tr) {
	cout << "--------" << endl;
	if (! tr.empty()) {
		rb_print(tr, tr.begin(), 0);
	}
	cout << "--------" << endl;
}

template <typename T>
void rb_right_rotate(RBTREE<T>& tr, typename RBTREE<T>::iterator y) {
	typename RBTREE<T>::iterator c;
	switch (tr.number_of_children(y)) {
	case 1: // c is nil, insert dummy as c
		c = tr.append_child(y, make_pair(RBFLAG::DUMMY, static_cast<T>(0)));
		break;
	case 2: // c is not nil
		c = tr.child(y, 1);
		break;
	default:
		cout << "*** rb_right_rotate: y=" << (*y).second << ": num child = " << tr.number_of_children(y) << endl;
		return;
	}
	typename RBTREE<T>::iterator x = tr.child(y, 0);
	typename RBTREE<T>::iterator a;
	typename RBTREE<T>::iterator b;
	switch (tr.number_of_children(x)) {
	case 0:
		a = tr.append_child(x, make_pair(RBFLAG::DUMMY, static_cast<T>(0)));
		b = tr.append_child(x, make_pair(RBFLAG::DUMMY, static_cast<T>(0)));
		break;
	case 1:
		a = tr.child(x, 0);
		b = tr.append_child(x, make_pair(RBFLAG::DUMMY, static_cast<T>(0)));
		break;
	case 2:
		a = tr.child(x, 0);
		b = tr.child(x, 1);
		break;
	default:
		cout << "*** rb_right_rotate: x=" << (*x).second << ": num child = " << tr.number_of_children(x) << endl;
		return;
	}
	// now rotate
	tr.move_before(y, x);
	tr.move_before(c, b);
	tr.move_after(a, y);
	assert(tr.number_of_children(y) == 2);
	assert(tr.number_of_children(x) == 2);
	// remove nils
	if ((*c).first == RBFLAG::DUMMY) {
		if ((*b).first == RBFLAG::DUMMY) {
			tr.erase(b);
		}
		tr.erase(c);
	}
}

template <typename T>
void rb_left_rotate(RBTREE<T>& tr, typename RBTREE<T>::iterator x) {
	if (tr.number_of_children(x) != 2) {
		cout << "*** rb_left_rotate: x=" << (*x).second << ": num child = " << tr.number_of_children(x) << endl;
		return;
	}
	typename RBTREE<T>::iterator a = tr.child(x, 0);
	typename RBTREE<T>::iterator y = tr.child(x, 1);
	typename RBTREE<T>::iterator b;
	typename RBTREE<T>::iterator c;
	switch (tr.number_of_children(y)) {
	case 0:
		b = tr.append_child(y, make_pair(RBFLAG::DUMMY, static_cast<T>(0)));
		c = tr.append_child(y, make_pair(RBFLAG::DUMMY, static_cast<T>(0)));
		break;
	case 1:
		b = tr.child(y, 0);
		c = tr.append_child(y, make_pair(RBFLAG::DUMMY, static_cast<T>(0)));
		break;
	case 2:
		b = tr.child(y, 0);
		c = tr.child(y, 1);
		break;
	default:
		cout << "*** rb_right_rotate: y=" << (*y).second << ": num child = " << tr.number_of_children(y) << endl;
		return;
	}
	// now rotate
	tr.move_after(x, y);
	tr.move_after(a, b);
	tr.move_before(c, x);
	assert(tr.number_of_children(y) == 2);
	assert(tr.number_of_children(x) == 2);
	// remove nils
	if ((*b).first == RBFLAG::DUMMY) {
		if ((*a).first == RBFLAG::DUMMY) {
			tr.erase(a);
		}
		tr.erase(b);
	}
	if ((*c).first == RBFLAG::DUMMY) {
		tr.erase(c);
	}
}

template <typename T>
void rb_fixup_after_insert(RBTREE<T>& tr, typename RBTREE<T>::iterator z) {
	typename RBTREE<T>::iterator p; // parent(z)
	while (((p = PARENT(tr, z)) != tr.end()) && ((*p).first == RBFLAG::RED)) {
		typename RBTREE<T>::iterator pp = PARENT(tr, p); // parent(p)
		assert(pp != tr.end());
		typename RBTREE<T>::iterator y; // z's uncle
		if (tr.index(p) == 0) { // cases L's: p is left child
			if (tr.number_of_children(pp) == 2) {
				y = tr.child(pp,1); // y is pp's right child
			} else {
				y = tr.end();
			}
			if ((y != tr.end()) && (*y).first == RBFLAG::RED) { // case L1: p is RED, y is RED
				(*p).first = RBFLAG::BLACK;
				(*y).first = RBFLAG::BLACK;
				(*pp).first = RBFLAG::RED;
				z = pp;
			} else {
				if (tr.index(z) == 1) { // case 2: p is RED, y is BLACK, z is right child
					z = p;
					rb_left_rotate(tr, z);
					p = PARENT(tr, z);
					assert(p != tr.end());
					assert(pp == PARENT(tr, p));
					assert(pp != tr.end());
				}
				// case 3: p is RED, y is BLACK, z is left child
				(*p).first = RBFLAG::BLACK;
				(*pp).first = RBFLAG::RED;
				rb_right_rotate(tr, pp);
				// no change for z
			}
		} else { // cases R's: p is right child
			y = tr.child(pp,0); // y is pp's left child
			assert(y != tr.end());
			if ((*y).first == RBFLAG::RED) { // case R1: p is RED, y is RED
				(*p).first = RBFLAG::BLACK;
				(*y).first = RBFLAG::BLACK;
				(*pp).first = RBFLAG::RED;
				z = pp;
			} else {
				if (tr.index(z) == 0) { // case R2: p is RED, y is BLACK, z is left child
					z = p;
					rb_right_rotate(tr, z);
					p = PARENT(tr, z);
					assert(p != tr.end());
					assert(pp == PARENT(tr, p));
					assert(pp != tr.end());
				}
				// case R3: p is RED, y is BLACK, z is right child
				(*p).first = RBFLAG::BLACK;
				(*pp).first = RBFLAG::RED;
				rb_left_rotate(tr, pp);
				// no change for z
			}
		}
	}
	// root node fix-up
	typename RBTREE<T>::iterator r = tr.begin();
	if (r != tr.end()) {
		assert((*r).first != RBFLAG::DUMMY);
		(*r).first = RBFLAG::BLACK;
	}
}

template <typename T>
typename RBTREE<T>::iterator rb_insert(RBTREE<T>& tr, typename RBTREE<T>::iterator it, T key) {
	while ((*it).second != key) {
		if (key < (*it).second) { // case B: move to left subtree
			if (tr.number_of_children(it) == 0) { // case B1: no child
				return tr.append_child(it, make_pair(RBFLAG::RED, key));
			}
			typename RBTREE<T>::iterator cit = tr.child(it, 0);
			if ((*cit).first == RBFLAG::DUMMY) { // case B3: only right child
				(*cit) = make_pair(RBFLAG::RED, key);
				return cit;
			}
			it = cit; // case B2, B4: left child only or two children
		} else { // case C: move to right subtree
			int n = tr.number_of_children(it);
			if (n == 0) { // case C1: no child
				tr.append_child(it, make_pair(RBFLAG::DUMMY, static_cast<T>(0))); // left child
				return tr.append_child(it, make_pair(RBFLAG::RED, key)); // right child
			}
			if (n == 1) { // case C2: left child only
				return tr.append_child(it, make_pair(RBFLAG::RED, key)); // right child
			}
			it = tr.child(it, 1); // case C3, C4: right child only or two children
		}
	}
	// case A: we found the value node
	return it;
}

template <typename T>
typename RBTREE<T>::iterator rb_insert(RBTREE<T>& tr, T key) {
	if (tr.empty()) {
		return tr.insert(tr.begin(), make_pair(RBFLAG::BLACK, key)); // root is BLACK
	} else {
		typename RBTREE<T>::iterator z = rb_insert(tr, tr.begin(), key);
		rb_fixup_after_insert(tr, z);
		return z;
	}
}

template <typename T>
typename RBTREE<T>::iterator rb_find(const RBTREE<T>& tr, typename RBTREE<T>::iterator it, T key) {
	while ((*it).second != key) {
		if (key < (*it).second) { // case B: move to left subtree
			if (tr.number_of_children(it) == 0) { // case B1: no child
				return tr.end();
			}
			typename RBTREE<T>::iterator cit = tr.child(it, 0);
			if ((*cit).first == RBFLAG::DUMMY) { // case B3: only right child
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
typename RBTREE<T>::iterator rb_find(const RBTREE<T>& tr, T key) {
	if (tr.empty()) {
		return tr.end();
	} else {
		return rb_find(tr, tr.begin(), key);
	}
}

template <typename T>
typename RBTREE<T>::iterator rb_find_min(const RBTREE<T>& tr, typename RBTREE<T>::iterator it) {
	assert(it != tr.end());
	while ((tr.number_of_children(it) != 0) && ((*tr.child(it, 0)).first != RBFLAG::DUMMY)) {
		it = tr.child(it, 0); // move to left child
	}
	return it;
}

template <typename T>
typename RBTREE<T>::iterator rb_find_min(const RBTREE<T>& tr) {
	if (tr.empty()) {
		return tr.end();
	} else {
		return rb_find_min(tr, tr.begin());
	}
}

template <typename T>
typename RBTREE<T>::iterator rb_find_max(const RBTREE<T>& tr, typename RBTREE<T>::iterator it) {
	assert(it != tr.end());
	while (tr.number_of_children(it) == 2) {
		it = tr.child(it, 1); // move to right child
	}
	return it;
}

template <typename T>
typename RBTREE<T>::iterator rb_find_max(const RBTREE<T>& tr) {
	if (tr.empty()) {
		return tr.end();
	} else {
		return rb_find_max(tr, tr.begin());
	}
}

template <typename T>
typename RBTREE<T>::iterator rb_find_succ(const RBTREE<T>& tr, typename RBTREE<T>::iterator it) { // successor
	if (tr.empty() || it == tr.end()) {
		return tr.end();
	}
	if (tr.number_of_children(it) == 2) { // if it has right sub-tree
		return rb_find_min(tr, tr.child(it, 1));
	} else { // no right sub-tree: find the lowest ancestor
		typename RBTREE<T>::iterator pit = PARENT(tr, it);
		while ((pit != tr.end()) && (tr.number_of_children(pit) == 2) && (tr.child(pit, 1) == it)) {
			it = pit;
			pit = PARENT(tr, it);
		}
		return pit;
	}
}

template <typename T>
typename RBTREE<T>::iterator rb_find_pred(const RBTREE<T>& tr, typename RBTREE<T>::iterator it) { // predecessor
	if (tr.empty() || it == tr.end()) {
		return tr.end();
	}
	if ((tr.number_of_children(it) > 0) && ((*tr.child(it, 0)).first != RBFLAG::DUMMY)) { // if it has left sub-tree
		return rb_find_max(tr, tr.child(it, 0));
	} else { // no left sub-tree: find the lowest ancestor
		typename RBTREE<T>::iterator pit = PARENT(tr, it);
		while ((pit != tr.end()) && (tr.number_of_children(pit) > 0) && (tr.child(pit, 0) == it)) {
			it = pit;
			pit = PARENT(tr, it);
		}
		return pit;
	}
}

template <typename T>
bool rb_validate(const RBTREE<T>& tr, const typename RBTREE<T>::iterator& it) {
	assert(it != tr.end());
	if ((*it).first == RBFLAG::DUMMY) {
		if (tr.number_of_children(it) != 0) {
			cout << "VALIDATE: dummy=" << (*it).second << " has " << tr.number_of_children(it) << " child(ren)" << endl;
			cout << "VALIDATE: parent = " << (*PARENT(tr, it)).second << endl;
			cout << "VALIDATE: child = ";
			for (int i = 0; i < tr.number_of_children(it); ++i) {
				cout << (*tr.child(it, i)).second << " ";
			}
			cout << endl;
			return false;
		}
		return true;
	}
	if (tr.number_of_children(it) > 2) {
		cout << "VALIDATE: node=" << (*it).second << " has " << tr.number_of_children(it) << " child(ren)" << endl;
		return false;
	}
	if (tr.number_of_children(it) == 1) { // only left child
		typename RBTREE<T>::iterator lit = tr.child(it, 0); // left child
		if ((*lit).first == RBFLAG::DUMMY) {
			cout << "VALIDATE: dangling sentinel found as left child of value=" << (*it).second << endl;
			return false;
		}
		if ((*lit).second > (*it).second) {
			cout << "VALIDATE: left-child=" << (*lit).second << ", node=" << (*it).second << endl;
			return false;
		}
		return rb_validate(tr, lit);
	} else if (tr.number_of_children(it) == 2) {
		typename RBTREE<T>::iterator rit = tr.child(it, 1); // right child
		if ((*rit).first == RBFLAG::DUMMY) {
			cout << "VALIDATE: illegal sentinel found as right child of value=" << (*it).second << endl;
			return false;
		}
		if ((*it).second > (*rit).second) {
			cout << "VALIDATE: node=" << (*it).second << ", right-child=" << (*rit).second << endl;
			return false;
		}
		if (rb_validate(tr, rit) != true) return false;
		typename RBTREE<T>::iterator lit = tr.child(it, 0); // left child
		if (((*lit).first != RBFLAG::DUMMY) && ((*lit).second > (*it).second)) {
			cout << "VALIDATE: left-child=" << (*lit).second << ", node=" << (*it).second << endl;
			return false;
		}
		return rb_validate(tr, lit);
	}
	return true;
}

template <typename T>
bool rb_validate_more(const RBTREE<T>& tr) {
	typename RBTREE<T>::iterator it;
	for (it = tr.begin(); it != tr.end(); ++it) {
		if ((*it).first == RBFLAG::RED) {
			typename RBTREE<T>::iterator p = PARENT(tr, it);
			if ((p != tr.end()) && ((*p).first == RBFLAG::RED)) {
				cout << "VALIDATE: double RED case: node=" << (*it).second << ", and parent=" << (*p).second << endl;
				return false;
			}
		}
	}
	return true;
}

template <typename T>
bool rb_validate(const RBTREE<T>& tr) {
	if (tr.empty()) {
		return true;
	} else {
		return rb_validate(tr, tr.begin()) && rb_validate_more(tr);
	}
}

std::random_device rd; // random device
std::default_random_engine re(rd()); // random engine

int main(int argc, char* argv[]) {
	std::string s = "ALGOR"; // ITHM";
	std::vector<char> v(s.begin(), s.end());
	RBTREE<char> tr;
	for (char& c : v) {
		rb_insert(tr, c);
	}
	// show it
	rb_print(tr);
	cout << "min = " << (*rb_find_min(tr)).second << endl;
	cout << "max = " << (*rb_find_max(tr)).second << endl;
	cout << "validate = " << std::boolalpha << rb_validate(tr) << endl;
	// find elements
	for (auto& e : v) {
		RBTREE<char>::iterator it = rb_find(tr, e);
		if (it != tr.end()) {
			cout << (*it).second;
			RBTREE<char>::iterator sit = rb_find_succ(tr, it);
			if (sit != tr.end()) {
				cout << ", succ = " << (*sit).second;
			}
			RBTREE<char>::iterator pit = rb_find_pred(tr, it);
			if (pit != tr.end()) {
				cout << ", pred = " << (*pit).second;
			}
			cout << endl;
		} else {
			cout << "not found: " << e << endl;
		}
	}
	// done
	return 0;
}
