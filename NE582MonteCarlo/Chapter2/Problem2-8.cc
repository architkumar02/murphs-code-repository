#include <iostream>
#include <cstdlib>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <time.h>

#define HOME 0
#define AWAY 1
#define SERIES 2
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
    double bin[3] = {0,0,0};       
    double bin2[3] = {0,0,0};
	
    srand(time(NULL));
	SeriesResult *s;
	for (i = 0; i < nSeries; i++){
		s = playBall();
		gamesAtHome += s->homeGames;
		gamesAway += s->awayGames;
		totalGamesPlayed += s->gamesPlayed;
		homeWins += s->homeWon;
        
        bin[HOME] += s->homeGames;
        bin[AWAY] += s->awayGames;
        bin[SERIES] += s->gamesPlayed;
        bin2[HOME] += (s->homeGames)*(s->homeGames);
        bin2[AWAY] += (s->awayGames)*(s->awayGames);
        bin2[SERIES] += (s->gamesPlayed)*(s->gamesPlayed);
		free(s);
	}

    // Printing the Results
    double xHat[3];
    double x2Hat[3];
    double sampleVar[3];
    double sampleSTD[3];
    double meanVar[3];
    double meanSTD[3];
    for(int i=0; i<3; i++){
        xHat[i] = bin[i]/(double)nSeries;
        x2Hat[i] = bin2[i]/(double)nSeries;
        sampleVar[i] = (double)nSeries/(double)(nSeries-1)*(x2Hat[i]-xHat[i]*xHat[i]);
        sampleSTD[i] = sqrt(sampleVar[i]);
        meanVar[i] = sampleVar[i]/(double)nSeries;
        meanSTD[i] = sqrt(meanVar[i]);
    }
	cout<<"Out of "<<nSeries<<" series, the home team won "<<homeWins<<endl;
	cout<<gamesAtHome<<" games where played at home and "<<gamesAway<<" where played away."<<endl;
    fprintf(stdout,"\tAverage number of games in a series: %5.3f +/- %5.3f\n",xHat[SERIES],meanSTD[SERIES]);
    fprintf(stdout,"\tHome Games: %5.3f +/- %5.3e\n",xHat[HOME],meanSTD[HOME]);
    fprintf(stdout,"\tAway Games: %5.3f +/- %5.3e\n",xHat[AWAY],meanSTD[AWAY]);
    
    cout<<"\tThe Fraction of Games at Home: "<<(double) gamesAtHome / (double) totalGamesPlayed<<endl;
	cout<<"\tThe Fraction of Games at Away: "<<(double) gamesAway / (double) totalGamesPlayed<<endl;
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
    int gameNum = 1;
    int maxGames = 7;
    fprintf(stdout,"\n");
	while (gameNum <= maxGames && homeWins < requiredWins && awayWins < requiredWins){
		int homeWon = 0;
        // Who will win the game?  Better watch 3 hours of TV to find out
		if ( homeWinProbability < (double) rand() / (double) RAND_MAX){
			homeWins++;
            homeWon = 1;
        }
		else
			awayWins++;
		
        // Where is the game played
		if (gameNum <= 2 && gameNum >= 1){
			result->homeGames++;
            fprintf(stdout,"Game %d was home, home won: %d\n",gameNum,homeWon);
        }
		else if (gameNum >=3 && gameNum <=5){
			result->awayGames++;
            fprintf(stdout,"Game %d was away, home won: %d\n",gameNum,homeWon);
        }
		else if (gameNum >=6 && gameNum <=7){
			result->homeGames++;
            fprintf(stdout,"Game %d was home, home won: %d\n",gameNum,homeWon);
        }
        else{
            fprintf(stderr,"Too many games for baseball");
            exit(EXIT_FAILURE);        
        }

		// Incrementing game pointer
		result->gamesPlayed++;
        gameNum ++;

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
