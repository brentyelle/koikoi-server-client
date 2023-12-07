#include "hanafuda-hands.hpp"
#include <algorithm>        //std::sort() -- according to C++ documentation, this runs with O(N*log N) efficiency.
#include <cassert>			//to force crash for debugging

/*  ##########################################################
	#========================================================#
	#            IMPLEMENTATION OF "Hand" CLASS              #
	#========================================================#
	########################################################## */

/* =====================================
CONSTRUCTOR & DESTRUCTOR
===================================== */
Hand::Hand() {
	cards.clear();
	return;
}
Hand::~Hand() {
	cards.clear();
	return;
}

/*  ========================================
INITIALIZER
========================================    */
void Hand::destroy() {
	cards.clear();                  //erase all cards in the hand
	return;
}

/* =====================================
CHECKING FUNCTIONS
===================================== */


int Hand::cardCount() const {									// returns the number of cards in the hand
	return cards.size();
};

bool Hand::isEmpty() const {									// returns true if the hand is empty, otherwise returns false
	return cards.empty();
};


// returns the number of cards in the hand that match the input month
int Hand::numOfThisMonth(MonthType m) const {
	int size = cards.size();
	int cardCt = 0;

	for (int i=0; i < size; i++) {                              // for each card in the hand
		if (cards[i].getMonth() == m) {
			cardCt++;
		}
	}

	return cardCt;
}

// returns the number of cards in the hand that match the input design
int Hand::numOfThisDesign(DesignType d) const {
	int size = cards.size();
	int cardCt = 0;

	for (int i=0; i < size; i++) {                              // for each card in the hand
		if (cards[i].getDesign() == d) {
			cardCt++;
		}
	}

	return cardCt;
}

// returns the number of cards in the hand that match the input month & design
int Hand::numOfThisCard(MonthType m, DesignType d) const {
	int size = cards.size();
	int cardCt = 0;

	for (int i=0; i < size; i++) {                                      // for each card in the hand
		if (cards[i].getMonth() == m && cards[i].getDesign() == d) {    /* (slightly inefficient due to calculating cards[i] twice, but I don't really mind here) */
			cardCt++;                                                   // if month AND design match, increase count
		}

		//efficiency: CHAFF is the only design that can repeat in a month, so if we're looking for a non-chaff and we found it, we can stop looking
		// (note for self: this means that December Chaff take the longest to evaluate.)
		if (d != CHAFF && cardCt == 1) {
			break;
		}
	}

	return cardCt;
}


// returns a copy of cards[index] -- WARNING: Will purposefully crash if given an invalid index!
CardType Hand::getCard(int index) const {
	if (index < 0 || index > static_cast<int>(cards.size())) {
		assert(false);
	}

	return CardType(cards[index]);			// copy constructor
}

// returns the first index of the card that matches the input month & design, or -1 if not found
int Hand::findFirstIndex(MonthType m, DesignType d) const {
	int size = cards.size();

	for (int i=0; i < size; i++) {										// for each card in the hand
		if (cards[i].getMonth() == m && cards[i].getDesign() == d) {	/* (slightly inefficient due to calculating cards[i] twice, but I don't really mind here) */
			return i;													// as soon as we find a perfect match, return the index where it was found
		}
	}

	// if we get here, the card was never found, so return -1
	return -1;
}


int Hand::findFirstMatch(MonthType m) const {
	int size = cards.size();

	for (int i=0; i < size; i++) {										// for each card in the hand
		if (cards[i].getMonth() == m) {									// as soon as we find a *month* match
			return i;													// return the index where it was found
		}
	}

	// if we get here, the card was never found, so return -1
	return -1;
}


// returns the number of cards in the hand that match the input month & design
bool Hand::hasRainy() const {
	int size = cards.size();

	for (int i=0; i < size; i++) {              // for each card in the hand
		if (cards[i].isRainy()) {               // if we found a rainy card
			return true;                        // then we're done
		}
	}
	
	// if we get here, then there were no rainy cards in the deck
	return false;
}

// returns true if the hand has the "instant win" condition: four pairs of cards of the same month
bool Hand::instantWin2222() const {
	int numPairs = 0;   // # of pairs of cards we have with the same month

	for (int m = JAN; m <= DEC; m++) {    // iterate through all 12 months
		if (numOfThisMonth(static_cast<MonthType>(m)) == 2) {             // if we have a pair
			numPairs++;
		}
		if (numPairs == 4) {                    // if we've counted 4 pairs
			return true;                        // then we're done
		}
	}

	// if we get here, then we never reached 4 pairs:
	return false;
}

// returns true if the hand has the "instant win" condition: four-of-a-kind of the same month
bool Hand::instantWin4() const {
	for (int m = JAN; m <= DEC; m++) {    // iterate through all 12 months
		if (numOfThisMonth(static_cast<MonthType>(m)) == 4) {             // if we found a 4-of-a-kind
			return true;                        // then we're done
		}
	}

	// if we get here, then we never found a 4-of-a-kind:
	return false;
}

/* =====================================
MUTATING FUNCTIONS
===================================== */
// sorts the cards in the hand using the operator < implemented in "hanafuda-card.hpp"
void Hand::sortCards() {
	/*
	int size = cards.size();
	int currentMin = 0;

	// SELECTION SORT: Iteratively swap every (next-)least element into the (next-)lowest index in the list.
	for (int i=0; i < size; i++) {
		// start the running minimum at the current index
		currentMin = i;
		// find the minimum card among the rest of the list
		for (int j=i+i; j < size; j++) {
			if (cards[j] < cards[currentMin]) {     // if we find a lesser card
				currentMin = j;                     // then update the current minimum
			}
		}
		// finally, swap the minimum into the (next-)earliest position if it's not already there
		if (i != currentMin) {
			CardType temp = cards[i];           //copy constructor
			cards[i] = cards[currentMin];       //copy constructor
			cards[currentMin] = temp;           //copy constructor
		}
		// at this point, all the cards from indices 0 thru i have been sorted
	}
	// at this point, all the cards in the hand (indices 0 thru size-1) have been sorted, so we're done
	*/

	std::sort(cards.begin(), cards.end(), std::less<CardType>());       // invokes the class-defined < operator

	return;
}

// adds a copy of the card given; also re-sorts the hand
void Hand::addCard(CardType newCard) {
	cards.push_back(newCard);                   // should use copy constructor, if I'm reading the C++ <vector> documentation right
	sortCards();
	return;
}

// removes the card at the given index, returning a copy of the removed card
CardType Hand::playCard(int index) {
	CardType playedCard = cards[index];         // copy constructor
	cards.erase(cards.begin()+index);           // erase the index-th element from the hand
	//sortCards();                              // unnecessary, since all unplayed cards will still be in order
	return playedCard;
}

/*  ##########################################################
	#========================================================#
	#          IMPLEMENTATION OF "ScorePile" CLASS           #
	#========================================================#
	########################################################## */


/* =====================================
LIGHT COMBINATIONS
===================================== */

// sakura viewing: has both Sake Cup (SEP SEED) and Curtain (MAR LIGHT), and does not have a rainy card
int ScorePile::sakuraViewing() const {
	if (    (numOfThisCard(SEP, SEED)  > 0)			//if we have the Sake Cup
		 && (numOfThisCard(MAR, LIGHT) > 0)       	//and the Curtain
		 && !hasRainy() )							//and no rainy cards
		return 5;
	else
		return 0;
}

// sakura viewing: has both Sake Cup (SEP SEED) and Moon (AUG LIGHT), and does not have a rainy card
int ScorePile::moonViewing() const {
	if (    (numOfThisCard(SEP, SEED)  > 0)			//if we have the Sake Cup
		 && (numOfThisCard(AUG, LIGHT) > 0)			//and the Moon
		 && !hasRainy() )							//and no rainy cards
		return 5;
	else
		return 0;
}

// light totals: 6 points for 3 lights, 7 or 8 points for 4 lights, and 15 points for all 5 lights
int ScorePile::totalLightScore() const {
	int numLights = numOfThisDesign(LIGHT);			//count # of Lights in score pile
	switch (numLights) {
		case 3:										// 3 lights:
			return 6;								// worth 6 points
			break;
		case 4:										// 4 lights
			if (numOfThisCard(NOV, LIGHT) > 0) {		// check if one of those 4 lights is Rain Man, and if so...
				return 7;               			// worth 7 points for "Rainy Four Lights"
			} else {								// otherwise...
				return 8;               			// worth 8 points for "Dry Four Lights"
			}
			break;
		case 5:										// all 5 lights:
			return 15;                  			// worth 15 points
			break;
		default:									// 2 lights or fewer:
			return 0;								// worth nothing
	}
}

/* =====================================
SEED COMBINATIONS
===================================== */

// 5 points if you have Boar (JUL SEED), Deer (OCT SEED), and Butterflies (JUN SEED)
int ScorePile::inoshikacho() const {
	if (numOfThisCard(JUL, SEED) + numOfThisCard(OCT, SEED) + numOfThisCard(JUN, SEED) == 3) {    // #boars + #deer + #butterflies (only 1 of each in the deck) must be 3
		return 5;
	} else {
		return 0;
	}
}

// 1 point for 5 Seed cards, and 1 more point for each additional Seed card
int ScorePile::totalSeedScore() const {
	int numSeeds = numOfThisDesign(SEED);   // count # of Seeds in score pile

	if (numSeeds >= 5) {
		return (numSeeds - 4);  // 1 point for 5, 2 points for 6, 3 points for 7, and so on.
	} else {
		return 0;               // 0 points for less than 5.
	}
}

/* =====================================
RIBBON COMBINATIONS
	Red         April, May, July, November
	Blue        June, September, October
	Poetry      January, February, March
===================================== */

// 5 points for having all 3 poetry ribbon cards
int ScorePile::threePoetryRibbons() const {
	if (    numOfThisCard(JAN, RIBBON)
		 && numOfThisCard(FEB, RIBBON)
		 && numOfThisCard(MAR, RIBBON) ) {
		return 5;
	} else {
		return 0;
	}
}

// 5 points for having all 3 blue ribbon cards
int ScorePile::threeBlueRibbons() const {
	if (    numOfThisCard(JUN, RIBBON)
		 && numOfThisCard(SEP, RIBBON)
		 && numOfThisCard(OCT, RIBBON) ) {
		return 5;
	} else {
		return 0;
	}
}

// 1 point for 5 Ribbon cards, and 1 more point for each additional Ribbon card
int ScorePile::totalRibbonScore() const {
	int numRibbons = numOfThisDesign(RIBBON);   // count # of Seeds in score pile

	if (numRibbons >= 5) {
		return (numRibbons - 4);    // 1 point for 5, 2 points for 6, 3 points for 7, and so on.
	} else {
		return 0;                   // 0 points for less than 5.
	}
}

/* =====================================
OTHER COMBINATIONS
===================================== */

// 4 cards for all 4 cards of a month
int ScorePile::fourOfAKind() const {
	int fullMonthCount = 0;                     // # of months for which we have all 4 cards

	for (int m = JAN; m <= DEC; m++) {    // iterate through all 12 months
		if (numOfThisMonth(static_cast<MonthType>(m)) == 4) {       // if the score pile has all 4 cards of that month
			fullMonthCount++;                   // then increase our counter
		}
	}
	return (fullMonthCount * 4);                // 4 points for each full month
}

int ScorePile::totalChaffScore() const {
	int numChaff = numOfThisDesign(CHAFF);   // count # of Chaff in score pile

	if (numChaff >= 10) {
		return (numChaff - 9);  // 1 point for 10, 2 points for 11, 3 points for 12, and so on.
//	} else if (numChaff == 0) {
//		return 10;				// 10 points for having zero chaff
	} else {
		return 0;               // 0 points for less than 10.
	}
}
