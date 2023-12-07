#include "hanafuda-deck.hpp"
#include <vector>
#include <cstdlib>	//rand(), srand()
#include <ctime>	//time(), to seed srand()

/*  ========================================
CONSTRUCTOR
Initailizes a deck of all 48 cards. Deck will not be shuffled!
========================================    */

DeckType::DeckType() {
	initialize();						// each new deck should begin with the full 48 cards
	return;
}

DeckType::~DeckType() {
	cards.clear();						// probably unnecessary, but can't hurt
	return;
}

void DeckType::initialize() {
	cards.clear();

	//January
	cards.emplace_back(JAN, LIGHT);		//crane & sun
	cards.emplace_back(JAN, RIBBON);	//poetry ribbon
	cards.emplace_back(JAN, CHAFF);
	cards.emplace_back(JAN, CHAFF);
	//February
	cards.emplace_back(FEB, SEED);		//bush warbler
	cards.emplace_back(FEB, RIBBON);	//poetry ribbon
	cards.emplace_back(FEB, CHAFF);
	cards.emplace_back(FEB, CHAFF);
	//March
	cards.emplace_back(MAR, LIGHT);		//curtain
	cards.emplace_back(MAR, RIBBON);	//poetry ribbon
	cards.emplace_back(MAR, CHAFF);
	cards.emplace_back(MAR, CHAFF);
	//April
	cards.emplace_back(APR, SEED);		//cuckoo
	cards.emplace_back(APR, RIBBON);	//red ribbon
	cards.emplace_back(APR, CHAFF);
	cards.emplace_back(APR, CHAFF);
	//May
	cards.emplace_back(MAY, SEED);		//bridge
	cards.emplace_back(MAY, RIBBON);	//red ribbon
	cards.emplace_back(MAY, CHAFF);
	cards.emplace_back(MAY, CHAFF);
	//June
	cards.emplace_back(JUN, SEED);		//butterflies
	cards.emplace_back(JUN, RIBBON);	//blue ribbon
	cards.emplace_back(JUN, CHAFF);
	cards.emplace_back(JUN, CHAFF);
	//July
	cards.emplace_back(JUL, SEED);		//boar
	cards.emplace_back(JUL, RIBBON);	//red ribbon
	cards.emplace_back(JUL, CHAFF);
	cards.emplace_back(JUL, CHAFF);
	//August
	cards.emplace_back(AUG, LIGHT);		//moon
	cards.emplace_back(AUG, SEED);		//geese
	cards.emplace_back(AUG, CHAFF);
	cards.emplace_back(AUG, CHAFF);
	//September
	cards.emplace_back(SEP, SEED);		//sake cup
	cards.emplace_back(SEP, RIBBON);	//blue ribbon
	cards.emplace_back(SEP, CHAFF);
	cards.emplace_back(SEP, CHAFF);
	//October
	cards.emplace_back(OCT, SEED);		//deer
	cards.emplace_back(OCT, RIBBON);	//blue ribbon
	cards.emplace_back(OCT, CHAFF);
	cards.emplace_back(OCT, CHAFF);
	//November
	cards.emplace_back(NOV, LIGHT);		//rain man
	cards.emplace_back(NOV, SEED);		//swallow
	cards.emplace_back(NOV, RIBBON);	//red ribbon
	cards.emplace_back(NOV, CHAFF);		//lightning
	//December
	cards.emplace_back(DEC, LIGHT);		//phoenix
	cards.emplace_back(DEC, CHAFF);
	cards.emplace_back(DEC, CHAFF);
	cards.emplace_back(DEC, CHAFF);

	return;
}

// remove all cards from the deck
void DeckType::destroy() {
	cards.clear();
	return;
}

/*  ========================================
CHECKING
========================================    */

// returns # of cards remaining in the deck
int DeckType::cardCount() const {
	return cards.size();						// built-in part of <vector>
}

// returns true if deck size==0, false otherwise
bool DeckType::isEmpty() const {
	return cards.empty();						// built-in part of <vector>
}

// looks at top card without (removing it) and returns it
CardType DeckType::topCard() const {
	CardType topcard = cards.back();			//copy constructor
	return topcard;
}

// for debug: count # of illegal cards in the deck
int DeckType::illegalCardCt() const {
	int size = cards.size();					// grab number to avoid repeated memory access
	int illegal_ct = 0;							// hopefully we'll never increment this

	for (int i = 0; i < size; i++) {			// for every card currently in the deck
		if (cards[i].isIllegal()) {				// if it's illegal
			illegal_ct++;						// then increment the counter
		}
	}

	return illegal_ct;
}

/*  ========================================
ACCESS
========================================    */

// pops a card and returns it
CardType DeckType::drawCard() {
	CardType topcard = cards.back();				// copy constructor
	cards.pop_back();								// remove card from deck
	return topcard;
}

// randomizes the order of the cards in the deck
void DeckType::shuffle() {
	int size = cards.size();
	int j;
	CardType temp = {JAN, CHAFF};

	//seed random number generation with current time
	std::srand(std::time(NULL));

	// for each card in the deck, swap its position with another random position in the deck
	for (int i = 0; i < size; i++) {
		j = std::rand() % size;						// generate a number between 0 and size-1
		temp = cards[j];							// copy constructor
		cards[j] = cards[i];						// copy constructor
		cards[i] = temp;							// copy constructor
	}

	// the deck should now be sufficiently randomized
	return;
}