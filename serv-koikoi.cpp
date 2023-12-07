#include "serv-koikoi.hpp"
extern "C" {
#include "csapp.h"
}
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdlib>	//rand(), srand()
#include <ctime>	//time(), to seed srand()
using namespace std;

/*  TYPES USED:
CardType		models a card, with comparison and name-printing functionality
DeckType		models a deck of cards, with drawing and shuffling functionality
Hand			models a hand, with card-checking, sorting, drawing, and playing functionality (also used to model the table, since the table needs no extra functionality not provided by Hand)
ScorePile		derived from Hand class, models a score pile with scoring functionality
*/


/* =====================================
MATCHING FUNCTIONS
===================================== */
// returns true if "tablecard" on the table can be matched by the card "matcher"
bool theseCardsMatch(const CardType &matcher, const CardType &tablecard) {
	if (matcher.isLightning()) {									// Lightning card can match anything (but can only *be matched* by other NOV cards)
		return true;
	} else if (matcher.getMonth() == tablecard.getMonth()) {		// "match" means to have the same month
		return true;
	}
	// otherwise
	return false;
}

// searches through the table and looks for all cards that can be matched by "matcher"
// UPDATE: the integer list "validTableCards" will be updated by reference with the list of indices of cards on the table that can be matched by "matcher"
// RETURN: boolean of whether any matchable cards on the table were found (i.e., whether "validTableCards" has any entries or not)
bool findMatches(const CardType &matcher, const Hand &table, std::vector<int> &validTableCards) {
	validTableCards.clear();										// make sure the list of valid cards is empty
	int tablesize = table.cardCount();								// increase efficiency with local variable

	for (int i=0; i < tablesize; i++) {								// for each card on the table
		if ( theseCardsMatch(matcher, table.getCard(i)) ) {			// if the chosen card matches card i on the table
			validTableCards.push_back(i);							// then add i to the list (updated by reference)
		}
	}
	return !validTableCards.empty();								// return whether the process above actually found any matches (true = match found, false = no matches found)
}

// returns true if findMatches(...) would return false for EVERY card in the hand; otherwise returns false
// "short-circuit"s if the hand or the table is empty, since of course nothing can be matched in those cases
bool noCardsToPlay(const Hand &hand, const Hand &table) {
	int handsize = hand.cardCount();
	std::vector<int> dummy;

	if (table.isEmpty() || hand.isEmpty()) {		// if either the hand or table are devoid of cards, we can't play anything this phase
		return true;
	}

	for (int i=0; i < handsize; i++) {
		if (findMatches(hand.getCard(i), table, dummy)) {			// if we ever find a card that can match something on the table
			return false;											// then we DO have a card to play, so return false
		}
	}

	// if we reach here, then no playable cards ever found, so return true
	return true;
}

// returns true if there findMatches(...) would return true
bool hasMatches(const CardType &matcher, const Hand &table) {
	std::vector<int> dummy;
	return findMatches(matcher, table, dummy);
}

/* =====================================
MOVING CARDS BETWEEN HANDS & TABLE & SCORE PILE
===================================== */

// deal n cards from the deck to the given hand
void dealCards(DeckType &deck, Hand &hand, int n) {
	int decksize = deck.cardCount();	// loop optimization

	// if the deck doesn't have enough cards to deal that many cards, then we've done something wrong, so crash here:
	if (n > decksize) {
		// std::cerr << "ERROR: Cannot draw " << n << " cards, as deck has only " << decksize << "cards." << std::endl;
		throw "ERROR: Tried to draw more cards than the deck has available.";
		return;
	}

	// otherwise, draw n cards from the deck and add them to the given hand (by reference)
	for (int i=0; i < n; i++) {			// this loop will occur 8 times
		hand.addCard(deck.drawCard());	// take a card from the deck and add it to the chosen hand
	}
	return;
}

// deal 8 cards to non-dealer, then 8 to table, then 8 to dealer
void setup(DeckType &deck, Hand &dealer, Hand &nondealer, Hand &table, ScorePile &playerPile, ScorePile &cpuPile) {
	// first, run cleanup() to initialize everything appropriately
	cleanup(deck, dealer, nondealer, table, playerPile, cpuPile);	// empties deck, both hands, and table
	// reset the deck and shuffle it
	deck.initialize();							// deck gets all 48 cards
	deck.shuffle();
	// finally, deal to non-dealer, then table, then dealer
	dealCards(deck, nondealer,	8);
	dealCards(deck, table, 		8);
	dealCards(deck, dealer,		8);
	// at this point, the deck should have 24 cards left
	if (deck.cardCount() != 24) {
		throw "ERROR: Something went wrong with the dealing process! Deck does not have 24 cards.";
	}
	return;
}

// resets & shuffles the deck, clears everyone's hands and score piles and the table
void cleanup(DeckType &deck, Hand &dealer, Hand &nondealer, Hand &table, ScorePile &playerPile, ScorePile &cpuPile) {
	// reset everyone's hands
	dealer.destroy();
	nondealer.destroy();
	table.destroy();
	// and score piles
	playerPile.destroy();
	cpuPile.destroy();
	// and the deck
	deck.destroy();
	return;
}

/* =====================================
PRINTING CURRENT GAME STATE
===================================== */

// prints a nice header for the round information

/*
	// send ending message to user
	write_buf.clear();
	write_buf += "Press Enter to quit.\n";
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	// get response from user
	Rio_readinitb(&rio, cfd);
	Rio_readlineb(&rio, read_buf, 20);
*/

void printRoundHeader(int cfd, int roundNumber) {
	string write_buf = "";

	write_buf.clear();
	write_buf += string("-----------------------------------------------\t");
    write_buf += string("                BEGIN ROUND ") + to_string(roundNumber) + string("\t");
	write_buf += string("-----------------------------------------------\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	return;
}

// prints which player is the dealer this round
void printDealer(int cfd, bool player_dealer) {
	string write_buf = "";
	write_buf.clear();

    if (player_dealer) {
        write_buf = string("You are the dealer for this round.\t");
    } else {
        write_buf = string("The CPU is the dealer for this round.\t");
    }

	// newline for spacing
	write_buf += string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
    return;
}

void printGetPoints(int cfd, const int score_to_add, const bool is_player) {
	string write_buf = "";
	write_buf.clear();

	if (is_player) {
		write_buf = string("You cash in your score pile and receive ") + to_string(score_to_add) + string(" points.\t");
	} else {	// it's the CPU's
		write_buf = string("The CPU cashes in its score pile and receives ") + to_string(score_to_add) + string(" points.\t");
	}

	// newline for spacing
	write_buf += string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	return;
}

// print a message that the round has ended w/o anyone scoring points
void printNoPoints(int cfd) {
	string write_buf = "";
	write_buf.clear();

	write_buf = string("This round has ended without any player scoring points!\tProceeding to next round...\t");

	// newline for spacing
	write_buf += string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	return;
}

// print a message showing both players' points at the end of a round
void printStandings(int cfd, const int playerScore, const int cpuScore) {
	string write_buf = "";
	write_buf.clear();

	write_buf =  string("---CURRENT STANDINGS---\t");
	write_buf += string("Your Score:  ") + to_string(playerScore) + string("\t");
	write_buf += string("CPU's Score: ") + to_string(cpuScore) + string("\t");

	// newline for spacing
	write_buf += string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	return;
}

// print a message at the conclusion of the game
void printFinalResults(int cfd, const int playerscore, const int cpuscore, const int totalrounds) {
	string write_buf = "";
	write_buf.clear();

	float player_avg = static_cast<float>(playerscore) / static_cast<float>(totalrounds);
	float cpu_avg    = static_cast<float>(cpuscore)    / static_cast<float>(totalrounds);


	write_buf  = string("-----------------------------------------------------\t");
	write_buf += string("FINAL RESULTS:\t\t");
	write_buf += string("Total # of Rounds:  ") + to_string(totalrounds) + string("\t");
	write_buf += string("Your Total Points:  ") + to_string(playerscore) + string(", average of ") + to_string(player_avg) + string(" points per round\t");
	write_buf += string("CPU's Total Points: ") + to_string(cpuscore)    + string(", average of ") + to_string(cpu_avg)    + string(" points per round\t");

	if (playerscore > cpuscore) {
		write_buf += string("\tYou are the winner! Congratulations!\t");
	} else if (playerscore < cpuscore) {
		write_buf += string("\tThe CPU wins. Better luck next time!\t");
	} else {
		write_buf += string("\tIt's a tie! How rare!\t");
	}

	// newline for spacing
	write_buf += string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	return;
}

// given the table and a list of indices from the table, prints all cards at those indices
void printMatchOptions(int cfd, const Hand &table, const std::vector<int> &validTableCards) {
	string write_buf = "";
	write_buf.clear();

	std::vector<int>::const_iterator listend = validTableCards.end();	// for more efficient end checking

	// print header
	write_buf = string("These are the cards on the table that you can match:\t");

	// loop through all valid card indices given in the list
	for (std::vector<int>::const_iterator it = validTableCards.begin(); it != listend; it++) {
		write_buf += string(" (") + to_string(*it) + string(")  ") + table.getCard(*it).cardName() + string("\t"); // print index in parentheses, then card name
	}
	// newline for spacing
	write_buf += string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	return;
}

// prints the hand of the player or CPU
void printHandState(int cfd, const Hand &hand, bool is_player) {
	string write_buf = "";
	write_buf.clear();

	int size = hand.cardCount();

	// print header
	write_buf = string("These are the cards in ");
	if (is_player) {
		write_buf += string("your ");
	} else {	// it's the CPU's
		write_buf += string("CPU's ");
	}
	write_buf += string("hand:\t");

	for (int i=0; i < size; i++) {	// for all indices of cards in the hand
		write_buf += string(" (") + to_string(i) + string(")  ") + hand.getCard(i).cardName() + string("\t"); // print index in parentheses, then card name
	}

	// newline for spacing
	write_buf += string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	return;
}

// prints all the cards on the table
void printTableState(int cfd, const Hand &table) {
	string write_buf = "";
	write_buf.clear();

	int size = table.cardCount();

	// print header
	write_buf = string("These are the cards on the table:\t");

	for (int i=0; i < size; i++) {	// for all indices of cards on the table
		write_buf += string(" (") + to_string(i) + string(")  ") + table.getCard(i).cardName() + string("\t"); // print index in parentheses, then card name
	}

	// newline for spacing
	write_buf += string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	return;
}

// print out all the cards in the score pile
void printScoreState(int cfd, const ScorePile &scorepile, bool opponentKK, bool is_player) {
	string write_buf = "";
	write_buf.clear();

	int size = scorepile.cardCount();
	// scorepile.sortCards();

	// print header
	write_buf = string("These are the cards in ");
	if (is_player) {
		write_buf += string("your ");
	} else {	// it's the CPU's
		write_buf += string("CPU's ");
	}
	write_buf += string("score pile:\t");

	for (int i=0; i < size; i++) {	// for all indices of cards in the score pile
		write_buf += string(" (") + to_string(i) + string(")  ") + scorepile.getCard(i).cardName() + string("\t"); // print index in parentheses, then card name
	}

	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	printScoreValue(cfd, scorepile, opponentKK, is_player);
	return; 
}

// prints current potential points in the given score pile
void printScoreValue(int cfd, const ScorePile &scorepile, bool opponentKK, bool is_player) {
	string write_buf = "";
	write_buf.clear();

	write_buf = string("Raw points in ");
	if (is_player) {
		write_buf += string("your ");
	} else {	// it's the CPU's
		write_buf += string("CPU's ");
	}

	write_buf += string("score pile: ")                           + to_string(scorepile.rawScore())             + string("\t");
	write_buf += string("With bonuses, this would be scored as ") + to_string(scorepile.finalScore(opponentKK)) + string(" points.\t");
	
	// newline for spacing
	write_buf += string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	
	return;
}

/* =====================================
PROMPTING THE USER
===================================== */
// prompt the user to call Koi-Koi or not, returing true if they did choose to call it
bool promptKoiKoi(int cfd, const ScorePile &playerPile, const int cpuScore, bool cpuCalledKK) {
	rio_t rio;
	string write_buf = "";
	char read_buf[MAXLINE];
	write_buf.clear();

	int user_choice = -1;
	bool to_return;

	write_buf  = string("You have made a new combo in your score pile! You can choose to end the game now, if you wish.\t");
	write_buf += string("If you do, then you will score ") + to_string(playerPile.finalScore(cpuCalledKK)) + string(" points. If you do not, then you must call \"Koi-Koi\".\t");
	write_buf += string("Calling \"Koi-Koi\" will continue the game so you can try to get more combos.\t");
	write_buf += string("However, if your opponent ends the round after this, you will score 0 points,\t");
	write_buf += string("and your opponent will score double their raw amount of points.\t");
	write_buf += string("If the CPU ended the round immediately, they would gain at least ") + to_string(cpuScore) + string(" points.\t");
	write_buf += string("\tWould you like to call \"Koi-Koi\", or end the round?\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

	write_buf.clear();
	do {
		// send prompt
		write_buf = string("Please enter [1] to call Koi-Koi, or [2] to end the round: \n");	//newline to end message
		Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

		// receive & interpret response
		Rio_readinitb(&rio, cfd);
		Rio_readlineb(&rio, read_buf, 20);
		sscanf(read_buf, "%i", &user_choice);

		// break if user entered a valid choice
		if (user_choice == 1 || user_choice == 2)
			break;
		// else
		write_buf.clear();
		write_buf = string("Your choice must be [1] for Koi-Koi, or [2] to end the round.\t");
		Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	} while (true);


	if (user_choice == 1) {				// if they call "Koi-Koi"
		write_buf.clear();
		write_buf = string("You say: \"Koi-Koi!\"\t");
		Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
		to_return = true;
	}
	else { // if (user_choice == 2)		// if they end the round
		write_buf.clear();
		write_buf = string("You choose to end the round.\t");
		Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
		to_return = false;
	}

	// newline for spacing
	write_buf.clear();
	write_buf = string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

	return to_return;
}

// prompt the user for which card in their hand they want to play
// ASSUMES THAT THE PLAYER HAS A MATCHABLE CARD!
int promptHandCardToPlay(int cfd, const Hand &hand, const Hand &table) {
	rio_t rio;
	string write_buf = "";
	char read_buf[MAXLINE];
	write_buf.clear();

	int chosen_index = -1;
	int handsize = hand.cardCount();
	std::vector<int> dummy;

	printTableState(cfd, table);		// print cards on the table
	printHandState(cfd, hand, true);	// print cards in player's hand

	// ask which one they want to match, not letting them continue until we get a satisfactory answer
	while (true) {
		// send prompt
		write_buf += string("Which card from your hand would you like to use for matching?\tEnter the index of the card: \n");	//newline to end message
		Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

		// get client input
		Rio_readinitb(&rio, cfd);
		Rio_readlineb(&rio, read_buf, 20);
		sscanf(read_buf, "%i", &chosen_index);		// try converting it to an integer

		// spacing
		write_buf.clear();
		write_buf = string("\t");
		Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

		write_buf.clear();
		if (read_buf[0] < '0' || read_buf[0] > '9') {			// if not a valid integer input
			write_buf = string("That is not a valid number. Please enter a digit.\t");
		} else if (chosen_index < 0 || chosen_index >= handsize) {	// if valid integer, but invalid card index
			write_buf = string("That is not a valid index for the cards in your hand. Please try again.\t");
		} else if (!findMatches(hand.getCard(chosen_index), table, dummy)) {	// if valid index, but no matching cards
			write_buf = string("The table has no cards that can match that one. Please enter a different card.\t");
		} else {	// valid index, and there is at least one matching card
			break;
		}
	}

	return chosen_index;
}

// prompt the user for which card on the table they want to match with their card
// ASSUMES THAT THERE IS A VALID MATCH!
int promptTableCardToMatch(int cfd, const CardType matcher, const Hand &table) {
	rio_t rio;
	string write_buf = "";
	char read_buf[MAXLINE];
	write_buf.clear();

	int chosen_index = -1;
	int tablesize = table.cardCount();

	std::vector<int> matchingCards;
	findMatches(matcher, table, matchingCards);
	printMatchOptions(cfd, table, matchingCards);

	while (true) {
		// send prompt
		write_buf  = string("You are matching the card: ") + matcher.cardName() + string("\t");
		write_buf += string("Which card from the table would you like to match with that card?\tEnter the index of the table card:\n");
		Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

		// get client input
		Rio_readinitb(&rio, cfd);
		Rio_readlineb(&rio, read_buf, 20);
		sscanf(read_buf, "%i", &chosen_index);		// try converting it to an integer

		// spacing
		write_buf.clear();
		write_buf = string("\t");
		Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

		write_buf.clear();
		if (read_buf[0] < '0' || read_buf[0] > '9') {									// if not a valid integer input
			write_buf = string("That is not a valid number. Please enter a digit.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
		} else if (chosen_index < 0 || chosen_index >= tablesize) {							// if valid integer, but invalid card index
			write_buf = string("That is not a valid index for the cards on the table. Please try again.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
		} else if ( std::find(matchingCards.begin(), matchingCards.end(), chosen_index) == matchingCards.end()) {	// if valid card index, but not matching
			write_buf = string("That card cannot be matched by your card. Please try again.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
		} else {	// if valid card
			break;
		}
	}

	return chosen_index;
}

int promptGiveUpCard(int cfd, const Hand &hand) {
	rio_t rio;
	string write_buf = "";
	char read_buf[MAXLINE];
	write_buf.clear();

	int chosen_index = -1;
	int handsize = hand.cardCount();
	
	// send prompt
	write_buf  = string("You cannot match any card from you hand with any card on the table,\t");
	write_buf += string("so instead, you must choose a card to give up to the table.\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

	printHandState(cfd, hand, true);	// print player's hand

	while (true) {
		// send prompt part 2
		write_buf.clear();
		write_buf = string("Which card to you choose to give up? Enter the index of the card: \n");

		// get client input
		Rio_readinitb(&rio, cfd);
		Rio_readlineb(&rio, read_buf, 20);
		sscanf(read_buf, "%i", &chosen_index);		// try converting it to an integer
		
		write_buf.clear();
		if (read_buf[0] < '0' || read_buf[0] > '9') {									// if not a valid integer input
			write_buf = string("\tThat is not a valid number. Please enter a digit.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
		} else if (chosen_index < 0 || chosen_index >= handsize) {							// if valid integer, but invalid card index
			write_buf = string("\tThat is not a valid index for the cards in your hand. Please try again.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
		} else {	// if valid card
			write_buf = string("You add the card to the table.\t");
			Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
			break;
		}
	}

	// newline for spacing
	write_buf.clear();
	write_buf = string("\t");
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

	return chosen_index;
}

/* =====================================
SIMULATING THE COMPUTER PLAYER
===================================== */

// simulates the computer choosing a card in their hand to match to one on the table
// 		hand_choice:  -1 if no can do, otherwise index of hand card to match  (update by reference)
// 		table_choice: -1 if no can do, otherwise index of table card to match (return value)
int computerChooseHandCardToPlay(const Hand &hand, const Hand &table, int &hand_choice) {
	int table_choice;					// return value: index of table card to match (-1 if not possible)
	int handsize  = hand.cardCount();	// # of cards in hand
	std::vector<int> matching_cards;		// list of indices of cards on the table that can be matched

	// randomize seed
	std::srand(std::time(NULL));

	// first, try to match a card from the table
	if (noCardsToPlay(hand, table)) {
		// printf("Computer has no cards in hand to match a table card.\tContinuing to next phase.\t");
		hand_choice  = -1;
		table_choice = -1;
		// printf("\t");
	} else {
		// first, randomly choose a card in hand to play
		while (true) {
			hand_choice = std::rand() % handsize;	// generate a valid index for a card in the CPU's hand
			if (findMatches(hand.getCard(hand_choice), table, matching_cards)) {	// if that card can match something on the table
				break;
			}
			// continue until we get a good card for hand_choice
		}

		// next, randomly choose a table card from matching_cards to match it to
		table_choice = std::rand() % matching_cards.size();	// grab a random index from matching_cards
		table_choice = matching_cards[table_choice];		// grab and grab the *table card* index stored at that matching_cards index
	}

	// hand_choice:  -1 if no can do, otherwise index of hand card to match  (update by reference)
	// table_choice: -1 if no can do, otherwise index of table card to match (return value)
	return table_choice;
}


// takes a card from the deck & the table, and returns the index of the card on the table to be matched to
int computerChooseTableCardToMatch(const CardType matcher, const Hand &table) {
	int table_choice;					// return value: index of table card to match (-1 if not possible)
	std::vector<int> matching_cards;		// list of indices of cards on the table that can be matched

	// randomize seed
	std::srand(std::time(NULL));

	if (table.isEmpty()) {				// if no cards in the table, then CPU can hardly match anything on it
		table_choice = -1;
		return table_choice;
	}

	// first, try to match a card from the table
	if (!findMatches(matcher, table, matching_cards)) {		// if no matchable cards on table
	//	printf("The CPU's card from the deck cannot match any card on the table.\tContinuing to next phase.\t");
		table_choice = -1;
	} else {	// if there are matchable cards on the table, their indices are now in matching_cards
		table_choice = matching_cards[std::rand() % matching_cards.size()];		// and grab a table card index stored in a random element of matching_cards
	}

	// table_choice: -1 if no can do, otherwise index of table card to match (return value)
	return table_choice;
}

// randomly choose a card from the hand to give up to the table
int computerChooseHandCard4Table(const Hand &hand) {
	std::srand(std::time(NULL));
	return rand() % hand.cardCount();		// return the index of a random card in the hand
}

// randomly choose whether or not to call Koi-Koi, based on current # of points
// the higher the computer's score, the more likely it will be to call Koi-Koi
bool computerCallKoiKoi(int cfd, const int rawscore) {
	string write_buf = "";
	write_buf.clear();

	std::srand(std::time(NULL));
	int chance = rand() % 100;				// generate a random number 0-99
	bool call_it = false;

	if (rawscore < 3 ) {
		call_it = chance < 25;	// 25% chance of calling Koi-Koi
	} else if (rawscore < 7) {
		call_it = chance < 50;	// 50% chance of calling Koi-Koi
	} else if (rawscore < 11) {
		call_it = chance < 75;	// 75% chance of calling Koi-Koi
	} else {	// if rawscore >= 11
		call_it = chance < 90;	// 90% chance of calling Koi-Koi
	}

	write_buf  = string("The CPU has collected a combo in its score pile, and it can end this round or call Koi-Koi.\t");
	write_buf += string("The CPU's choice is: ");
	if (call_it) {
		write_buf += string("Koi-Koi!\t");
	} else {
		write_buf += string("End the round.\t");
	}
	write_buf += string("\t");

	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

	return call_it;
}

/* =====================================
WRAPPER FUNCTIONS FOR PLAYER & COMPUTER TURNS
===================================== */

// runs all the functions for the player's turn
void doPlayerTurn(int cfd, Hand &hand, DeckType &deck, Hand &table, ScorePile &pile, bool &called_KK, bool &end_round, const bool cpu_KK, const int cpu_score) {
	string write_buf = "";
	write_buf.clear();

	int score_before = pile.rawScore();		// starting score in the score pile
	int score_after  = score_before;
	int hand_index  = -1;
	int table_index = -1;

	// PHASE 1: check cards in the hand to match to the table
	// PHASE 1: if no matches, then choose a card to put on the table
	if (noCardsToPlay(hand, table)) {
		hand_index = promptGiveUpCard(cfd, hand);
		table.addCard(hand.playCard(hand_index));
	}
	// PHASE 1: if there is a possible match choose a hand card & table card, then put both in the score pile
	else {
		hand_index = promptHandCardToPlay(cfd, hand, table);
		table_index = promptTableCardToMatch(cfd, hand.getCard(hand_index), table);
		write_buf  = string("Your reveal this card from your hand:   ") + hand.getCard(hand_index).cardName()   + string("\t");
		write_buf += string("You match it to this card on the table: ") + table.getCard(table_index).cardName() + string("\t");
		write_buf += string("Both cards are put in your score pile.\t\t");
		Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

		pile.addCard(hand.playCard(hand_index));			// take matched card from hand and put it in score pile
		pile.addCard(table.playCard(table_index));			// take matched card from table and put it in score pile
	}
	
	write_buf.clear();
	// PHASE 2: draw a card from the deck
	CardType deck_card = deck.drawCard();
	write_buf = string("You reveal this card from the deck:      ") + deck_card.cardName() + string("\t");
	// PHASE 2: if no matches, then put the deck card onto the table
	if (!hasMatches(deck_card, table)) {
		write_buf += string("You cannot match this card with any card on the table, so it is added to the table.\t\t");
		table.addCard(deck_card);
	}
	// PHASE 2: if it matches something on the table, choose a table card, then put both in the score pile
	else {
		table_index = promptTableCardToMatch(cfd, deck_card, table);
		write_buf += string("You match this card with the table card: ") + table.getCard(table_index).cardName() + string("\t");
		write_buf += string("Both cards are put in your score pile.\t\t");
		pile.addCard(deck_card);
		pile.addCard(table.playCard(table_index));
	}
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

	// PHASE 3: check score pile for koi-koi
	score_after = pile.rawScore();										// get the current score points
	if (score_before != score_after) {									// if score points have changed
		end_round = !promptKoiKoi(cfd, pile, cpu_score, cpu_KK);				// determine if player wants to call Koi-Koi or not; if he doesn't, the round ends
		called_KK = called_KK || !end_round;							// if player hasn't called Koi-Koi before, then update it to be so, if the player did so
	} else {
		end_round = false;												// if no update to score pile, then no koi-koi vs. end game decision
	}

	write_buf.clear();

	return;
}

// automates all the functions for the computer's turn
void doComputerTurn(int cfd, Hand &hand, DeckType &deck, Hand &table, ScorePile &pile, bool &called_KK, bool &end_round) {
	string write_buf = "";
	write_buf.clear();

	int score_before = pile.rawScore();		// starting score in the score pile
	int score_after  = score_before;
	int hand_index  = -1;
	int table_index = -1;

	// PHASE 1: check cards in the hand to match to the table
	table_index = computerChooseHandCardToPlay(hand, table, hand_index);
	// PHASE 1: if no matches, then choose a card to put on the table
	if (table_index == -1) {
		hand_index = computerChooseHandCard4Table(hand);	// select card from hand to add to the table
		write_buf  = string("Computer cannot match any card from its hand with any card on the table,\t");
		write_buf += string("so instead it sacrifices this card to the table: ") + hand.getCard(hand_index).cardName() + string("\t\t");
		table.addCard(hand.playCard(hand_index));			// take card from hand and give it to the table
	}
	// PHASE 1: if it matches something on the table, choose a hand card & table card, then put both in the score pile
	else {
		write_buf  = string("Computer reveals this card from its hand: ") + hand.getCard(hand_index).cardName()   + string("\t");
		write_buf += string("It matches it to this card on the table:  ") + table.getCard(table_index).cardName() + string("\t");
		write_buf += string("Both cards are put in the CPU's score pile.\t\t");
		pile.addCard(hand.playCard(hand_index));			// take matched card from hand and put it in score pile
		pile.addCard(table.playCard(table_index));			// take matched card from table and put it in score pile
	}
	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());

	// PHASE 2: draw a card from the deck
	write_buf.clear();
	CardType deck_card = deck.drawCard();
	write_buf = string("Computer reveals this card from the deck: ") + deck_card.cardName() + string("\t");

	// PHASE 2: if no matches, then put the deck card onto the table
	table_index = computerChooseTableCardToMatch(deck_card, table);
	if (table_index == -1) {
		write_buf += string("Computer cannot match this card with any card on the table, so it is added to the table.\t\t");
		table.addCard(deck_card);
	}
	// PHASE 2: if it matches something on the table, choose a table card, then put both in the score pile
	else {
		write_buf += string("Computer matches this card with the table card: ") + table.getCard(table_index).cardName() + string("\t");
		write_buf += string("Both cards are put in the CPU's score pile.\t\t");
		pile.addCard(deck_card);
		pile.addCard(table.playCard(table_index));
	}

	Rio_writen(cfd, strdup(write_buf.c_str()), write_buf.length());
	write_buf.clear();

	// PHASE 3: check score pile for koi-koi
	score_after = pile.rawScore();									// get the current score points
	if (score_before != score_after) {								// if score points have changed
		end_round = !computerCallKoiKoi(cfd, score_after);				// determine if computer wants to call Koi-Koi or not; if it doesn't, the round ends
		called_KK = called_KK || !end_round;						// if computer hasn't called Koi-Koi before, then update it to be so, if the CPU kept the round going
	} else {
		end_round = false;											// if no update to score pile, then no koi-koi vs. end game decision
	}

	return;
}