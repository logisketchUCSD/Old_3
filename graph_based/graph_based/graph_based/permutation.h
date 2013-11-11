#ifndef PERMUTATION_H
#define PERMUTATION_H

#define PERMUTATION_MAX		6

#include <vector>

using namespace std;

class Permutation {
private:
	int iN;
	int iTotal;
	int iOffset;

public:
	Permutation(int iN);
	int n(void) const {
		return (iN);
	}
	int total(void) const {
		return (iTotal);
	}
	int offset(void) const {
		return (iOffset);
	}
	vector<int> operator[](int iIndex);
};

#endif // PERMUTATION_H