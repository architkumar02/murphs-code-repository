#include "HaltonSequence.h"
#include <vector>
#include <cstdlib>
#include <cmath>
#define DEFAULT_NUM 10000
using namespace std;
HaltonSequence::HaltonSequence(int base):base(base){
	sequence = Sequence(DEFAULT_NUM);
	index = 0;
}
/**
 * HaltonSequence  of base base, with n trials
 */
HaltonSequence::HaltonSequence(int base, int n):base(base){
	sequence = Sequence(n);
	index = 0;
}
HaltonSequence::~HaltonSequence(){}
/**
 * Returns a random sequence from the Halton Sequence
 */
double HaltonSequence::nextRandom(){
	return sequence[++index];
}

/**
 * Returns the nth Halton Sequence
 *
 * Based on Pseudo Code from Wikipedia
 */
double HaltonSequence::random(int n){
	double result = 0;
	double f = 1.0 / (double)base;
	while (n > 0){
		result += f*(n % base);
		n = (int) floor((double) n /(double) base);
		f = f / base;
	}
	return result;
}
/**
 * Computes the Halton Sequence of n Elements
 *
 * This bit of code be be otpimized, current O(n^2) when it could be O(n)
 */
vector<double> HaltonSequence::Sequence(int n){
	if (sequence.size() > (unsigned int)n)
		return sequence;
	
	vector<double> s;
	int i;
	for (i = 1; i<=n; i++)
		s.push_back(random(i));
	return s;
}
