#ifndef HANAFUDA_DECK_H
#define HANAFUDA_DECK_H

#include "hanafuda-card.hpp"
#include <vector>

class DeckType {
protected:
	std::vector<CardType> cards;
public:
	/*  ========================================
	CONSTRUCTOR & DESTRUCTOR
	Initailizes a deck of all 48 cards. Deck will not be shuffled!
	========================================    */
	DeckType();						// uses initialize() to make a full standard deck; does NOT shuffle the deck
	~DeckType();					// empties all cards from the deck before deletion

	/*  ========================================
	INITIALIZER
	========================================    */
	void initialize();				// (re-)initializes a proper deck of all 48 standard hanafuda cards; does NOT shuffle the deck
	void destroy();					// empties all cards from the deck

	/*  ========================================
	CHECKING FUNCTIONS
	========================================    */
	int cardCount() const;			// returns # of cards remaining in the deck
	bool isEmpty() const;			// returns true if deck size==0, false otherwise
	CardType topCard() const;		// looks at top card without (removing it) and returns it
	void printCards() const;		// prints the entire deck's contents
	int illegalCardCt() const;		// returns true if there is an illegal card somewhere in the deck (debug only)

	/*  ========================================
	MUTATING FUNCTIONS
	========================================    */
	CardType drawCard();			// pops a card and returns it
	void shuffle();					// randomizes the order of the cards in the deck
};

#endif