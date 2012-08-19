#include "Discrepancy.h"
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;

Discrepancy::Discrepancy(std::vector<double> data) : data(data){
	// Initilization done by initilizaiton list
}
Discrepancy::~Discrepancy(){}

double Discrepancy::StarDiscrepancy(){
	double star = 0;
	double fracExpected;
	double val;
	unsigned int i;
	
	std::sort(data.begin(),data.end());

	for(i=0; i < data.size(); i++){
		// Just Before
		fracExpected = (double) i/(double) data.size();
		val = fracExpected - data[i];
		star = AbsMax(star,val);
		// Just After
		fracExpected = (double) (i+1)/(double) data.size();
		val = fracExpected - data[i];
		star = AbsMax(star,val);
	}
	return star;
}

double Discrepancy::AbsMax(const double a, const double b){
	double absA = abs(a);
	double absB = abs(b);
	// Ternary conditional
	return absA < absB ? absB : absA; 
}

