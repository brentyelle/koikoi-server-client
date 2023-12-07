#ifndef HANAFUDA_CARD_H
#define HANAFUDA_CARD_H

#include <string>

enum MonthType {	// "suit" of the cards: there will be 4 of each month in a full deck
	JAN		=1,			// light, ribbon, chaff, chaff
	FEB		=2,			// seed, ribbon, chaff, chaff
	MAR		=3,			// light, ribbon, chaff, chaff
	APR		=4,			// seed, ribbon, chaff, chaff
	MAY		=5,			// seed, ribbon, chaff, chaff
	JUN		=6,			// seed, ribbon, chaff, chaff
	JUL		=7,			// seed, ribbon, chaff, chaff
	AUG		=8,			// light, seed, chaff, chaff
	SEP		=9,			// seed, ribbon, chaff, chaff
	OCT		=10,		// seed, ribbon, chaff, chaff
	NOV		=11,		// light, seed, ribbon, chaff*		(chaff* is the Lightning wild card)
	DEC		=12			// light, chaff, chaff, chaff		(NB: the "special" December chaff is not implemented, as it is not special in Koi-Koi, nor in the vast majority of Hanafuda games)
};

enum DesignType {	// "design" of the cards -- their inner values are set so that they can never overlap with the above MonthType constants.
	LIGHT	=100,		// 5 count
	SEED	=101,		// 9 count
	RIBBON	=102,		// 10 count
	CHAFF	=103		// 24 count
};

class CardType {
private:
	MonthType	month;
	DesignType	design;

public:
	/*  ========================================
	CONSTRUCTORS
	No default constructor, so that all cards __must__ be constructed explicitly.
	========================================    */
	CardType(MonthType m, DesignType d) {	// value constructor
		month = m;
		design = d;
		return;
	}

	CardType(const CardType &rhs) {			// copy constructor
		month = rhs.month;
		design = rhs.design;
		return;
	}

	// ASSIGNMENT OPERATOR: Copies the month and design.
	CardType& operator=(const CardType &rhs) {
		month = rhs.month;
		design = rhs.design;
		return *this;
	}

	/*  ========================================
	GETTERS
	========================================    */
	MonthType getMonth() const {
		return month;
	}
	DesignType getDesign() const {
		return design;
	}

	/*  ========================================
	BOOLEAN OPERATORS
	Defined inline due to brevity.
	========================================    */

	// EQUALITY: Defined as month and design both being equal.
	bool operator==(const CardType rhs) const {
		return (month == rhs.month && design == rhs.design);
	}

	// NON-EQUALITY: If one or both of month and design are non-equal.
	bool operator!=(const CardType rhs) const {
		return (month != rhs.month || design != rhs.design);
	}

	// LESS THAN: Determined firstly based on month, and if months are equal, then by design.
	bool operator<(const CardType rhs) const {
		// cast months & designs to integers to ensure comparability
		int monthL  = static_cast<int>(month);
		int monthR  = static_cast<int>(rhs.month);
		int designL = static_cast<int>(design);
		int designR = static_cast<int>(rhs.design);

		if (monthL < monthR) {
			return true;
		} else if (monthL > monthR) {
			return false;
		} else {  //if monthL == monthR
			return (designL < designR);
		}
	}

	/*  ========================================
	FUNCTIONS FOR LIGHTS
	There are 5 unique Seed cards, spread across 5 months.
		JAN     Crane & Sun
		FEB     --
		MAR     Curtain
		APR     --
		MAY     --
		JUN     --
		JUL     --
		AUG     Full Moon
		SEP     --
		OCT     --
		NOV     Rain Man
		DEC     Phoenix
	========================================    */
	bool isLight() const;			// matches any Light
	bool isDryLight() const;		// matches all Lights except Rain Man
	bool isRainMan() const;			// matches only Rain Man

	/*  ========================================
	FUNCTIONS FOR RIBBONS
	There 3 types of Ribbon cards: "red", "blue", and "poetry". There are 10 ribbon cards total, found across 10 months.
		Red       April, May, July, November
		Blue        June, September, October
		Poetry      January, February, March
	========================================    */
	bool isRibbon() const;				// matches any kind of ribbon
	bool isRedRibbon() const;			// matches only APR, MAY, JUL, NOV ribbons
	bool isBlueRibbon() const;			// matches only JUN, SEP, OCT ribbons
	bool isPoetryRibbon() const;		// matches only JAN, FEB, MAR ribbons

	/*  ========================================
	FUNCTIONS FOR SEEDS
	There are 9 unique Seed cards, spread across 9 months.
		JAN     --
		FEB     Bush Warbler
		MAR     --
		APR     Cuckoo
		MAY     Bridge
		JUN     Butterflies
		JUL     Boar
		AUG     Geese
		SEP     Sake Cup
		OCT     Deer
		NOV     Swallow
		DEC     --
	========================================    */
	bool isSeed() const;            //matches any kind of seed
	bool isBushWarbler() const;
	bool isCuckoo() const;
	bool isBridge() const;
	bool isButterflies() const;
	bool isBoar() const;
	bool isGeese() const;
	bool isSakeCup() const;
	bool isDeer() const;
	bool isSwallow() const;

	/*  ========================================
	FUNCTIONS FOR CHAFF
	There are 24 Chaff cards in the game, spread across all 12 months.
	January through October have 2 chaff each, November has 1, and December has 3.
	The November chaff is known as the "Lightning" card, and it has special properties.
	Rarely, some games will also treat one of the Paulownia chaff as special, but Koi-Koi does not,
	so I have not implemented it here.
	========================================    */
	bool isChaff() const;               // matches any kind of chaff
	bool isPlainChaff() const;          // matches any kind of chaff except lightning
	bool isLightning() const;           // matches only lightning

	/*  ========================================
	OTHER CARD ID-ing FUNCTIONS
	========================================    */
	bool isRainy() const;									// matches only rain man & lightning
	bool isIllegal() const;									// debug only: returns true if card has an illegal combination of month+design
	bool isThisCard(MonthType m, DesignType d) const {		// returns true if the card matches both inputs, otherwise false
		return (month == m && design == d);
	}
	
	/*  ========================================
	NAME-CREATING FUNCTIONS
	========================================    */

	std::string cardName() const;			//
	std::string cardMonthName() const;		// "January", "February", etc.
	std::string cardFlowerName() const;		// "Pine", "Plum", "Cherry", etc.
	std::string cardDesignName() const;		// "Crane & Sun", "Bush Warbler", "Blue Ribbon", "Chaff"
	std::string cardDesignTypeName() const;	// "Light", "Seed", "Ribbon", "Chaff"
};

#endif