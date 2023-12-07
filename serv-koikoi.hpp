#ifndef KOIKOI_H
#define KOIKOI_H

#include <vector>
#include <string>
#include "hanafuda-hands.hpp"
#include "hanafuda-deck.hpp"
extern "C" {
#include "csapp.h"
}

/*  TYPES USED:
CardType		models a card, with comparison and name-printing functionality
DeckType		models a deck of cards, with drawing and shuffling functionality
Hand			models a hand, with card-checking, sorting, drawing, and playing functionality (also used to model the table, since the table needs no extra functionality not provided by Hand)
ScorePile		derived from Hand class, models a score pile with scoring functionality
*/

/* =====================================
MATCHING FUNCTIONS
===================================== */
bool theseCardsMatch(const CardType &matcher, const CardType &tablecard);												// returns true if the two cards match (= have the same month)
bool findMatches(const CardType &matcher, const Hand &table, std::vector<int> &validTableCards);						// find if there are any matches on the table for the matcher card
bool noCardsToPlay(const Hand &hand, const Hand &table);                                                                // find if a hand has any cards that can match a table card
bool hasMatches(const CardType &matcher, const Hand &table);                                                            // find if a card will have any matches from findMatches()

/* =====================================
MOVING CARDS BETWEEN HANDS & TABLE & SCORE PILE
===================================== */
void dealCards(DeckType &deck, Hand &hand, int n);                       												// deal n cards from the deck to the given hand
void setup(DeckType &deck, Hand &dealer, Hand &nondealer, Hand &table, ScorePile &playerPile, ScorePile &cpuPile);		// deal 8 cards to non-dealer, then 8 to table, then 8 to dealer
void cleanup(DeckType &deck, Hand &dealer, Hand &nondealer, Hand &table, ScorePile &playerPile, ScorePile &cpuPile);	// resets & shuffles the deck, clears everyone's hands and

/* =====================================
PRINTING CURRENT GAME STATE
===================================== */
// announcements
void printRoundHeader(int cfd, int roundNumber);																					// prints out a basic header for the round
void printDealer(int cfd, bool player_dealer);																			        // prints out who the dealer is for the round
void printGetPoints(int cfd, const int score_to_add, const bool is_player);                                                      // prints a message for when points are scored
void printNoPoints(int cfd);																									// prints a message for when no points are scored this round
void printStandings(int cfd, const int playerScore, const int cpuScore);                                                         // prints out a message for the score at the end of a round
void printFinalResults(int cfd, const int playerscore, const int cpuscore, const int totalrounds);								// prints out a message declaring the final winner

// printing lists of cards
void printMatchOptions(int cfd, const Hand &table, const std::vector<int> &validTableCards);										// prints out the list of matchable table cards
void printHandState(int cfd, const Hand &hand);																					// prints out all the cards in the hand
void printTableState(int cfd, const Hand &table);																				// prints out all the cards on the table
void printScoreState(int cfd, const ScorePile &scorepile, bool opponentKK, bool is_player);										// prints out all the cards in the score pile
void printScoreValue(int cfd, const ScorePile &scorepile, bool opponentKK, bool is_player);										// prints out how much the cards in the score pile are worth

/* =====================================
INTERACTING WITH THE USER
===================================== */
bool promptKoiKoi(int cfd, const ScorePile &playerPile, const int cpuScore, bool cpuCalledKK);		                            // prompts the user to call Koi-Koi or not (returns true if they call KK)
int  promptHandCardToPlay(int cfd, const Hand &hand, const Hand &table);															// prompts the user to choose a card in their hand to play
int  promptTableCardToMatch(int cfd, const CardType matcher, const Hand &table);													// prompts the user to match a deck card to a table card

/* =====================================
SIMULATING THE COMPUTER PLAYER
===================================== */
int computerChooseHandCardToPlay(const Hand &hand, const Hand &table, int &hand_choice);							    // has the computer match a hand card to a table card
int computerChooseTableCardToMatch(const CardType matcher, const Hand &table);										    // has the computer match a deck card to a table card
int computerChooseHandCard4Table(const Hand &hand);                                                                     // has the computer choose a card to give up to the table
bool computerCallKoiKoi(int cfd, const int rawscore);                                                                            // has the computer choose whether to call Koi-Koi or not

/* =====================================
WRAPPERS FOR BOTH PLAYERS' TURNS
===================================== */
void doPlayerTurn  (int cfd, Hand &hand, DeckType &deck, Hand &table, ScorePile &pile, bool &called_KK, bool &end_round,         // wrapper for all stuff the player does on his turn
                        const bool cpu_KK, const int cpu_score);    // extra things needed for promptKoiKoi()
void doComputerTurn(int cfd, Hand &hand, DeckType &deck, Hand &table, ScorePile &pile, bool &called_KK, bool &end_round);	    // wrapper for all stuff the cpu does on its turn

#endif