#ifndef DISCREPANCY_H
#define DISCREPANCY_H
#include <vector>

class Discrepancy {

	public:
		Discrepancy(std::vector<double> data);
		virtual ~Discrepancy();		
		/* Calculates the Star Discrepancy */
		double StarDiscrepancy();
	
	private:
		double AbsMax(const double a, const double b);
		std::vector<double>  data;
};

#endif
