#ifndef LCG_H
#define LCG_H

class LCG {

	public:
		LCG(unsigned long a, unsigned long b, unsigned long m, unsigned long i);
		~LCG();
		double randomLCG();	// Returns a Random Variable According to the LCG
		void LCGSequence();  // Displays the complete sequenc of the LCG

	private:
		unsigned long incrementLCG();
		unsigned long a;		// 
		unsigned long b;
		unsigned long m;		// Period of the distirbution
		unsigned long i;		// Current Value for Bootstrap
};
#endif
