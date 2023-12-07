#ifndef HANAFUDA_HANDS_H
#define HANAFUDA_HANDS_H

#include <vector>
#include "hanafuda-card.hpp"

class Hand {
protected:
	std::vector<CardType> cards;	//contains the actual cards
public:
	/* =====================================
	CONSTRUCTOR & DESTRUCTOR
	===================================== */
	Hand();     // initialized with zero cards
	~Hand();    // empties all cards from the hand before deletion

	/*  ========================================
	INITIALIZER
	========================================    */
	void destroy();											// empties all cards from the hand

	/* =====================================
	CHECKING FUNCTIONS
	===================================== */
	int cardCount() const;									// returns the number of cards in the hand
	bool isEmpty() const;									// returns true if the hand is empty, otherwise returns false

	int numOfThisMonth(MonthType m) const;					// returns the number of cards in the hand that match the input month
	int numOfThisDesign(DesignType d) const;				// returns the number of cards in the hand that match the input design
	int numOfThisCard(MonthType m, DesignType d) const;		// returns the number of cards in the hand that match the input month & design

	CardType getCard(int index) const;						// returns a copy of cards[index] -- WARNING: Will purposefully crash if given an invalid index!
	int findFirstIndex(MonthType m, DesignType d) const;	// returns the first index of the card that matches the input month & design, or -1 if not found
	int findFirstMatch(MonthType m) const;					// returns the first index of the card that matches the input month, or -1 if not found

	bool hasRainy() const;									// returns true if there is a rainy card present (Rain Man or Lightning), false otherwise
	bool instantWin2222() const;							// returns true if the hand has the "instant win" condition: four pairs of cards of the same month
	bool instantWin4() const;								// returns true if the hand has the "instant win" condition: four-of-a-kind of the same month

	/* =====================================
	MUTATING FUNCTIONS
	===================================== */
	void sortCards();										// sorts (std::sort) the cards in the hand using the operator< implenented in "hanafuda-card.hpp"
	void addCard(CardType newCard);							// adds a copy of the card given; also re-sorts the hand
	CardType playCard(int index);							// removes the card at the given index, returning a copy of the removed card; also re-sorts the hand
};


class ScorePile : public Hand {
public:
	/* =====================================
	SCORE-TOTALING FUNCTIONS
	Implemented in-line because they should not differ between any implementations.
	===================================== */
	// calculates raw # of points in the score pile, without any bonus multipliers
	int rawScore() const {
		int score = 0;
		score += sakuraViewing();
		score += moonViewing();
		score += totalLightScore();
		score += inoshikacho();
		score += totalSeedScore();
		score += threePoetryRibbons();
		score += threeBlueRibbons();
		score += totalRibbonScore();
		score += fourOfAKind();
		score += totalChaffScore();
		return score;
	}

	// calculates the # of points gained for this round, equal to rawScore() times bonus multipliers
	// argument: whether or not the opposing player has called "Koi-Koi" during this round
	int finalScore(bool opponentKK) const {
		int score = rawScore();				// first, get the raw score
		if (score >= 7) { score *= 2; }		// if the raw score is >= 7, double it
		if (opponentKK) { score *= 2; }		// if the opponent (CPU or player) has called Koi-Koi this round, double the score (possibly again)
		return score;
	}

	/* =====================================
	INDIVIDUAL SCORING FUNCTIONS
	These will look at the contents of the score pile and return an integer corresponding to the score for that category.
	===================================== */

	// LIGHT COMBINATIONS
	int sakuraViewing() const;
	int moonViewing() const;
	int totalLightScore() const;
	// seed combinations
	int inoshikacho() const;
	int totalSeedScore() const;
	// poetry combinations
	int threePoetryRibbons() const;
	int threeBlueRibbons() const;
	int totalRibbonScore() const;
	// other combinations
	int fourOfAKind() const;
	int totalChaffScore() const;	// points for 10+ chaff
};

#endif