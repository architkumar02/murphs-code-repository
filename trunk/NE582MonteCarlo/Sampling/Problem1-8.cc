#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Discrepancy.h"
using namespace std;

int main(){

	double d[] = {0.12509, 0.15127, 0.08885, 0.94886, 0.54036};
	vector<double> data(d,d+sizeof(d)/sizeof(double));
	Discrepancy star = Discrepancy(data);

	fprintf(stdout,"Star Disgrepancy is %f\n",star.StarDiscrepancy());

}
