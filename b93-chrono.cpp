#ifndef __cplusplus
#error This file works only with C++
#endif

#include <iostream>
#include <chrono>
#include <thread> // for sleep_for()
using namespace std;
using namespace std::chrono;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

int main(void) {
	steady_clock::time_point start = steady_clock::now();
	this_thread::sleep_for(milliseconds(1000));
	steady_clock::time_point end = steady_clock::now();
	long long elapsed_msec = duration_cast<milliseconds>(end - start).count();
	cout << "elapsed time = " << elapsed_msec << " msec" << endl;
	return 0;
}
