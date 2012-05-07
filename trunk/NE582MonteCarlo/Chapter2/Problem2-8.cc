#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <time.h>
using namespace std;


typedef struct {
	int homeGames;
	int awayGames;
	int gamesPlayed;
	int homeWon;
}SeriesResult;

void initilizeSeries(SeriesResult *s);
SeriesResult *playBall();

int main(int argc, char **argv){
	int nSeries;
	if (argc != 2)
		nSeries = 1000;
	else
		nSeries = atoi(argv[1]);

	int i;
	int homeWins = 0;
	int gamesAtHome = 0;
	int gamesAway = 0;
	int totalGamesPlayed = 0;
	srand(time(NULL));
	SeriesResult *s;
	for (i = 0; i < nSeries; i++){
		s = playBall();
		gamesAtHome += s->homeGames;
		gamesAway += s->awayGames;
		totalGamesPlayed += s->gamesPlayed;
		homeWins += s->homeWon;
		free(s);
	}

	cout<<"Out of "<<nSeries<<" series, the home team won "<<homeWins<<endl;
	cout<<gamesAtHome<<" games where played at home and "<<gamesAway<<" where played away."<<endl;
	cout<<"The Fraction of Games at Home: "<<(double) gamesAtHome / (double) totalGamesPlayed<<endl;
	cout<<"The Fraction of Games at Away: "<<(double) gamesAway / (double) totalGamesPlayed<<endl;
}

/**
 * World Series is a best of seven; once a team wins 4 games it is over.
 * The location of the games are 2 games at A, 3 games at B, and then
 * (if necessary) the final two games at A.
 */
SeriesResult *playBall(){

	// Initilizaiton
	SeriesResult *result = (SeriesResult*) malloc(sizeof(SeriesResult));
	initilizeSeries(result);
	int homeWins = 0;
	double homeWinProbability = 0.5;
	int awayWins = 0;				
	int requiredWins = 4;

	while (homeWins < requiredWins && awayWins < requiredWins){
		// Where is the game played
		if (result->gamesPlayed <= 2)
			result->homeGames++;
		else if (result->gamesPlayed <=5)
			result->awayGames++;
		else
			result->homeGames++;

		// Incrementing game pointer
		result->gamesPlayed++;

		// Who will win the game?  Better watch 3 hours of TV to find out
		if ( homeWinProbability < (double) rand() / (double) RAND_MAX)
			homeWins++;
		else
			awayWins++;
	}
	result->homeWon = (homeWins > awayWins);
	return result;	
}
void initilizeSeries(SeriesResult *s){
	s->homeGames = 0;
	s->awayGames = 0;
	s->gamesPlayed = 0;
	s->homeWon = 0;		// 0 for false
}
