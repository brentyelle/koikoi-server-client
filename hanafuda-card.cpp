#include "hanafuda-card.hpp"
#include <string>
#include <iostream>
#define CARDNAMELENGTH 50

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
bool CardType::isLight() const {			// matches any Light
	return (design == LIGHT);
}

bool CardType::isDryLight() const {		// matches any Light except Rain Man
	return (design == LIGHT && month != NOV);
}

bool CardType::isRainMan() const {		// matches only Rain Man
	return (design == LIGHT && month == NOV);
}

/*  ========================================
FUNCTIONS FOR RIBBONS
There 3 types of Ribbon cards: "red", "blue", and "poetry". There are 10 ribbon cards total, found across 10 months.
	Red         April, May, July, November
	Blue        June, September, October
	Poetry      January, February, March
========================================    */

// 3 poetry, 3 blue, 4 plain (10 total); none in August or December
bool CardType::isRibbon() const {
	return (design == RIBBON);
}

// April, May, July, November
bool CardType::isRedRibbon() const {
	if (design != RIBBON)
		return false; // disqualified if not a ribbon card
	switch (month) {
		case APR:
		case MAY:
		case JUL:
		case NOV:
			return true;
		default:
			return false;
	}
}     

// June, September, October
bool CardType::isBlueRibbon() const {
	if (design != RIBBON)
		return false; // disqualified if not a ribbon card
	switch (month) {
		case JUN:
		case SEP:
		case OCT:
			return true;
		default:
			return false;
	}
}

// January, February, March
bool CardType::isPoetryRibbon() const {
	if (design != RIBBON)
		return false; // disqualified if not a ribbon card
	switch (month) {
		case JAN:
		case FEB:
		case MAR:
			return true;
		default:
			return false;
	}
}

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

// bush warbler, cuckoo, bridge, butterflies, boar, geese, sake, deer, swallow (9 total)
bool CardType::isSeed() const {
	return (design == SEED);
}

bool CardType::isBushWarbler() const {
	return (month == FEB && design == SEED);
}

bool CardType::isCuckoo() const {
	return (month == APR && design == SEED);
}

bool CardType::isBridge() const {
	return (month == MAY && design == SEED);
}

bool CardType::isButterflies() const {
	return (month == JUN && design == SEED);
}

bool CardType::isBoar() const {
	return (month == JUL && design == SEED);
}

bool CardType::isGeese() const {
	return (month == AUG && design == SEED);
}

bool CardType::isSakeCup() const {
	return (month == SEP && design == SEED);
}

bool CardType::isDeer() const {
	return (month == OCT && design == SEED);
}

bool CardType::isSwallow() const {
	return (month == NOV && design == SEED);
}


/*  ========================================
FUNCTIONS FOR CHAFF
========================================    */

// 2 for each month January - October, 1 for November (Lightning), 3 for December
bool CardType::isChaff() const {
	return (design == CHAFF);
}

// all chaff except lightning
bool CardType::isPlainChaff() const {
	return (design == CHAFF && month != NOV);
}

// only lightning
bool CardType::isLightning() const {
	return (design == CHAFF && month == NOV);
}

/*  ========================================
OTHER ID-ing FUNCTIONS
========================================    */

// rain man, lightning
bool CardType::isRainy() const {
	return (month == NOV && (design == CHAFF || design == LIGHT));
}

// checks for illegal cards (debug only)
bool CardType::isIllegal() const {
	switch (month) {
		case JAN:
		case MAR:
			return (design == SEED);
		case FEB:
		case APR:
		case MAY:
		case JUN:
		case JUL:
		case SEP:
		case OCT:
			return (design == LIGHT);
		case AUG:
			return (design == RIBBON);
		case NOV:
			return false;
		case DEC:
			return (design == SEED || design == RIBBON);
		default:
			std::cerr << "[ERROR in CardType::cardMonthName() in hanafuda.cpp -- Invalid MONTH]" << std::endl;
			return true;
	}
}

/*  ========================================
NAME-PRINTING FUNCTIONS
========================================    */

// "January", "February", etc.
std::string CardType::cardMonthName() const {
	switch (month) {
		case JAN:
			return static_cast<std::string>("January");
		case FEB:
			return static_cast<std::string>("February");
		case MAR:
			return static_cast<std::string>("March");
		case APR:
			return static_cast<std::string>("April");
		case MAY:
			return static_cast<std::string>("May");
		case JUN:
			return static_cast<std::string>("June");
		case JUL:
			return static_cast<std::string>("July");
		case AUG:
			return static_cast<std::string>("August");
		case SEP:
			return static_cast<std::string>("September");
		case OCT:
			return static_cast<std::string>("October");
		case NOV:
			return static_cast<std::string>("November");
		case DEC:
			return static_cast<std::string>("December");
		default:
			return static_cast<std::string>("[ERROR in CardType::cardMonthName() in hanafuda.cpp -- Invalid MONTH]");
	}
}

// "Pine", "Plum", "Cherry", etc.
std::string CardType::cardFlowerName() const {
	switch (month) {
		case JAN:
			return static_cast<std::string>("Pine");
		case FEB:
			return static_cast<std::string>("Plum");
		case MAR:
			return static_cast<std::string>("Cherry");
		case APR:
			return static_cast<std::string>("Wisteria");
		case MAY:
			return static_cast<std::string>("Iris");
		case JUN:
			return static_cast<std::string>("Peony");
		case JUL:
			return static_cast<std::string>("Clover");
		case AUG:
			return static_cast<std::string>("Silvergrass");
		case SEP:
			return static_cast<std::string>("Chrysanthemum");
		case OCT:
			return static_cast<std::string>("Maple");
		case NOV:
			return static_cast<std::string>("Willow");
		case DEC:
			return static_cast<std::string>("Paulownia");
		default:
			return static_cast<std::string>("[ERROR in CardType::cardFlowerName() in hanafuda.cpp -- Invalid MONTH]");
	}
}

// "Crane & Sun", "Bush Warbler", "Blue Ribbon", "Chaff"
std::string CardType::cardDesignName() const {
	// check for chaff designs
	if (isChaff()) {
		if (isPlainChaff()) {
			return static_cast<std::string>("Plain Chaff");
		} else if (isLightning()) {
			return static_cast<std::string>("Lightning");
		} else {
			return static_cast<std::string>("[ERROR in CardType::cardDesignName() in hanafuda.cpp -- Invalid CHAFF]");
		}
	}

	// check for ribbon designs
	else if (isRibbon()) {
		if (isRedRibbon()) {
			return static_cast<std::string>("Red Ribbon");
		} else if (isBlueRibbon()) {
			return static_cast<std::string>("Blue Ribbon");
		} else if (isPoetryRibbon()) {
			return static_cast<std::string>("Poetry Ribbon");
		} else {
			return static_cast<std::string>("[ERROR in CardType::cardDesignName() in hanafuda.cpp -- Invalid RIBBON]");
		}
	}
	// check for seed designs
	/*
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
	*/
	else if (isSeed()) {
		switch (month) {
			case FEB:
				return static_cast<std::string>("Bush Warbler");
			case APR:
				return static_cast<std::string>("Cuckoo");
			case MAY:
				return static_cast<std::string>("Bridge");
			case JUN:
				return static_cast<std::string>("Butterflies");
			case JUL:
				return static_cast<std::string>("Boar");
			case AUG:
				return static_cast<std::string>("Geese");
			case SEP:
				return static_cast<std::string>("Sake Cup");
			case OCT:
				return static_cast<std::string>("Deer");
			case NOV:
				return static_cast<std::string>("Swallow");
			default:	//JAN, MAR, DEC
				return static_cast<std::string>("[ERROR in CardType::cardDesignName() in hanafuda.cpp -- Invalid SEED]"); 
		}
	}
	// check for light designs
	/*
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
	*/
	else if (isLight()) {
		switch (month) {
			case JAN:
				return static_cast<std::string>("Crane & Sun");
			case MAR:
				return static_cast<std::string>("Curtain");
			case AUG:
				return static_cast<std::string>("Full Moon");
			case NOV:
				return static_cast<std::string>("Rain Man");
			case DEC:
				return static_cast<std::string>("Phoenix");
			default:
				return static_cast<std::string>("[ERROR in CardType::cardDesignName() in hanafuda.cpp -- Invalid LIGHT]"); 
		}
	}
	// catch any errors
	else {
		return static_cast<std::string>("[ERROR in CardType::cardDesignName() in hanafuda.cpp] -- Invalid DESIGN"); 
	}
	
}

// "Light", "Seed", "Ribbon", "Chaff"
std::string CardType::cardDesignTypeName() const {
	switch (design) {
		case CHAFF:
			return static_cast<std::string>("Chaff");
		case RIBBON:
			return static_cast<std::string>("Ribbon");
		case SEED:
			return static_cast<std::string>("Seed");
		case LIGHT:
			return static_cast<std::string>("Light");
		default:
			return static_cast<std::string>("[ERROR in CardType::cardDesignTypeName() in hanafuda.cpp]");
	}
}

std::string CardType::cardName() const {
	std::string cardname = "";	//string to be returned

	cardname += cardMonthName() + " (" + cardFlowerName() + ") " + cardDesignTypeName();	// show month, flower, and design type
	if (!isPlainChaff()) {																	// and if it's not a plain chaff:
		cardname += " ~ " + cardDesignName();												// then show the specific name
		if (isRainy()) {																	// and if it's a rainy card:
			cardname += " [Rainy]";															// then tell the player as much
		}
	}
	return cardname;	
}