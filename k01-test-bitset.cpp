#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <vector>
#include <deque>
#include <bitset>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <cstring>
using namespace std;
using namespace std::chrono;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

void print(const std::vector<bool>& vb) {
	for (size_t i = 0; i < vb.size(); ++i) {
		cout << vb[i];
	}
}

void print(const std::deque<bool>& qb) {
	for (size_t i = 0; i < qb.size(); ++i) {
		cout << qb[i];
	}
}

int main(int argc, char* argv[]) {
	// std::bitset test
	std::bitset<27> bitVec; // bit vector
	bitVec.reset();
	bitVec.set(size_t(8), true);
	bitVec |= 0b10000000; // bit #7 is set
	bitVec |= 0x0010; // bit #4 is set
	cout << "bit vector: size=" << bitVec.size() << endl;
	cout << "\tdata=" << bitVec << endl;
	// std::vector<bool> test
	std::vector<bool> vecBool { false, true, false, true }; // vector with size 4
	cout << "std::vector<bool>: size=" << vecBool.size() << ", capacity=" << vecBool.capacity() << endl;
	cout << "\tdata=";
	print(vecBool);
	cout << endl;
	vecBool.resize(72);
	vecBool[8] = true;
	vecBool[7] = true;
	vecBool[4] = true;
	cout << "std::vector<bool>: size=" << vecBool.size() << ", capacity=" << vecBool.capacity() << endl;
	cout << "\tdata=";
	print(vecBool);
	cout << endl;
	// std::deque<bool> test
	std::deque<bool> deqBool { false, true, false, true }; // vector with size 4
	cout << "std::deque<bool>: size=" << deqBool.size() << endl;
	cout << "\tdata=";
	print(deqBool);
	cout << endl;
	deqBool.resize(27);
	deqBool[8] = true;
	deqBool[7] = true;
	deqBool[4] = true;
	cout << "std::deque<bool>: size=" << deqBool.size() << endl;
	cout << "\tdata=";
	print(deqBool);
	cout << endl;
	cout << "\t&deqBool[0] = " << &deqBool[0] << endl;
	cout << "\t&deqBool[1] = " << &deqBool[1] << endl;
	// test index
	int a[4] = { 0, 1, 2, 3 };
	cout << "a[2] = " << a[2] << endl;
	cout << "2[a] = " << 2[a] << endl;
	// done
	return 0;
}
