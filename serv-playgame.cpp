#include "serv-koikoi.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
extern "C" {
#include "csapp.h"
}
using namespace std;

int serviceKoiKoi (int cfd) {
	// current round state
	int currRound;                      // the current round that is in play (from 1 to TOTALROUNDS)
	int TOTALROUNDS         = 0;        // total number of rounds that will be played
	bool player_is_dealer;              // whether the player is the dealer this round
	bool player_KK          = false;    // whether the player has called Koi-Koi this round
	bool cpu_KK             = false;    // whether the CPU has called Koi-Koi this round
	bool round_should_end   = false;    // whether the current round should end before the next player's turn
	bool player_ended_round;            // whether the player just ended the current round
	bool cpu_ended_round;               // whether the cpu just ended the current round
	int addscore;						// # of points to be added to point total
	// networking variables
	rio_t rio;
	string write_buf = "";
	char read_buf[MAXLINE] = "\0";

	// current turn state
	bool player_turn        = false;    // whether it is the player's turn
	
	// objects to store gameplay stuff
	DeckType theDeck;
	Hand playerHand, cpuHand, tableHand;
	ScorePile playerPile, cpuPile;
	int playerScore     = 0, cpuScore     = 0;    // current points scored
	//int playerPotential = 0, cpuPotential = 0;    // most recently-calculated points gainable from the score pile

	// solicit for # of rounds
	write_buf.clear();
	write_buf += string("How many rounds of koi-koi would you like to play?\t");	// start building string for writing
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	do {	//infinite loop until the user cooperates
		// send message to client
		write_buf.clear();
		write_buf += string("Enter a number 1-12:\n");								// newline to end this message
		Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());				// send the message
		// read client's response
		Rio_readinitb(&rio, cfd);											// set up for reading from client
		Rio_readlineb(&rio, read_buf, 5);									// read from client
		sscanf(read_buf, "%i", &TOTALROUNDS);								// see if it's an integer
		// if invalid response, prompt client to insert again
		if (TOTALROUNDS > 12 || TOTALROUNDS < 1) {
			write_buf.clear();
			write_buf += "You must enter a number between 1 and 12, inclusive.\t";
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
		}
	} while (TOTALROUNDS > 12 || TOTALROUNDS < 1);

	srand(time(NULL));
	player_is_dealer = static_cast<bool>(rand() % 2);   // randomly choose if player will be dealer or not

	for (currRound = 1; currRound <= TOTALROUNDS; currRound++) {
		// print info for this round
		printRoundHeader(cfd, currRound);
		printDealer(cfd, player_is_dealer);

		// reset round-ending states
		player_KK           = false;
		cpu_KK              = false;
		round_should_end    = false;
		player_ended_round  = false;
		cpu_ended_round     = false;
		//playerPotential     = 0;
		//cpuPotential        = 0;
		addscore 			= 0;

		// set up for start of round, which includes cleanup(...);
		if (player_is_dealer) {
			setup(theDeck, playerHand, cpuHand, tableHand, playerPile, cpuPile);
		} else {    // if cpu is dealer
			setup(theDeck, cpuHand, playerHand, tableHand, playerPile, cpuPile);
		}

		// the dealer goes first
		player_turn = player_is_dealer;

		write_buf.clear();
		// check for instant-win combos
		if (playerHand.instantWin2222()) {
			write_buf += string("You were dealt four pairs of matching cards--an instant-win combo!\tYou score 6 points, and this round is over.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
			playerScore += 6;
			printStandings(cfd, playerScore, cpuScore);
			continue;
		} else if (playerHand.instantWin4()) {
			write_buf += string("You were dealt four of a kind--an instant-win combo!\tYou score 6 points, and this round is over.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
			playerScore += 6;
			printStandings(cfd, playerScore, cpuScore);
			continue;
		} else if (cpuHand.instantWin2222()) {
			write_buf += string("The CPU was dealt four pairs of matching cards--an instant-win combo!\tThe CPU scores 6 points, and this round is over.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
			cpuScore += 6;
			printStandings(cfd, playerScore, cpuScore);
			continue;
		} else if (cpuHand.instantWin4()) {
			write_buf += string("The CPU was dealt four of a kind--an instant-win combo!\tThe CPU scores 6 points, and this round is over.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
			cpuScore += 6;
			printStandings(cfd, playerScore, cpuScore);
			continue;
		} else if (tableHand.instantWin2222()) {
			write_buf += string("The Table was dealt four pairs of matching cards--an instant-win combo!\tThis deal is null and void, and the round will be re-dealt.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
			currRound--;	// repeat this round
			continue;
		} else if (tableHand.instantWin4()) {
			write_buf += string("The Table was dealt four of a kind--an instant-win combo!\tThis deal is null and void, and the round will be re-dealt.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
			currRound--;	// repeat this round
			continue;
		}

		// play the round
		while (!round_should_end) {		// until one player says to stop
			// printTableState(tableHand);						// print the table cards

			// simulate both players' turns
			if (player_turn) {	// on player's turn
				doPlayerTurn(cfd, playerHand, theDeck, tableHand, playerPile, player_KK, player_ended_round, cpu_KK, cpuPile.finalScore(player_KK));
				printScoreState(cfd, playerPile, cpu_KK, true);      // print player's current potential score
				round_should_end = player_ended_round;
			} else {			// on computer's turn
				doComputerTurn(cfd, cpuHand,  theDeck, tableHand, cpuPile,    cpu_KK,    cpu_ended_round);
				printScoreState(cfd, cpuPile, player_KK, false);     // print CPU's current potential score
				round_should_end = cpu_ended_round;
			}

			// if the deck runs out, there round ends
			if (theDeck.isEmpty()) {
				round_should_end = true;
			}
			if (!round_should_end) {
				player_turn = (player_turn == false);	// toggle player_turn true <--> false for next iteration
				write_buf.clear();
				write_buf += "-----------------------------\t";
				Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
			}
		}

		// if player ended the round, then player scores points
		if (player_ended_round) {
			addscore = playerPile.finalScore(cpu_KK);		// calculate points to add
			printGetPoints(cfd, addscore, true);					// print point-getting message (true == player)
			playerScore += addscore;						// add points to total
			player_is_dealer = true;						// winner becomes next dealer
		}
		// if cpu ended the round, then player scores points
		else if (cpu_ended_round) {
			addscore = cpuPile.finalScore(player_KK);		// calculate points to add
			printGetPoints(cfd, addscore, false);				// print point-getting message (false == CPU)
			cpuScore    += addscore;   						// add points to total
			player_is_dealer = false;						// winner becomes next dealer
		}
		// otherwise, nobody gets any points
		else {
			printNoPoints(cfd);
			// player_is_dealer remains the same as it was
		}

		// print standings
		printStandings(cfd, playerScore, cpuScore);
	}

	printFinalResults(cfd, playerScore, cpuScore, TOTALROUNDS);

	// send ending message to user
	write_buf.clear();
	write_buf += string("Enter 99 to quit.\n");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	// get response from user
	Rio_readinitb(&rio, cfd);
	Rio_readlineb(&rio, read_buf, 20);
	// (we don't actually care what the response is)
	return 0;
}