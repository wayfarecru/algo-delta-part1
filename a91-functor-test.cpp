#include <iostream>
#include <cmath> // for fabsf()
using namespace std;
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#if defined(_MSC_VER)
#pragma warning(disable: 4388 4389)
#endif

struct Equal {
	float eps;
	bool operator()(const float& lhs, const float& rhs) {
		return fabsf(lhs - rhs) < eps;
	}
};

int main(void) {
	Equal equal;
	equal.eps = 0.00001F;
	cout << std::boolalpha << equal(0.0100F, 0.0101F) << endl; // not equal
	equal.eps = 0.01F;
	cout << std::boolalpha << equal(0.0100F, 0.0101F) << endl; // equal
	return 0;
}
