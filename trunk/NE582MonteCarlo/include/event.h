#ifndef event_h
#define event_h
typedef struct{
	double x;			// Store the position
	double mu;			// Store the cosine of the direction
	double w;			// Weight of event
	int E;				// Energy group index of the particle
	int numCollision;	// number of collions so far
	int eventID;		// Event ID
} event_t;

#endif