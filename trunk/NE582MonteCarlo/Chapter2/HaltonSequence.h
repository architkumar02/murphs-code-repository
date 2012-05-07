#ifndef HALTONSEQUENCE_H
#define HALTONSEQUENCE_H
#include <vector>
using namespace std;

class HaltonSequence{
	public:
		HaltonSequence(int base);
		HaltonSequence(int base, int n);
		~HaltonSequence();
		vector<double> Sequence(int numbers);
		double random(int n);
		double nextRandom();
	private:
		int base;
		int index;
		vector<double> sequence;
};
#endif
