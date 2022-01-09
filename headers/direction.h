#ifndef DIRECTION_H
#define DIRECTION_H

#include <cstdlib>

//--------------------------------------------------------------------------------

#include "domain.h"

/*
	A class that implements the direction of the bot.

	Directions are:
	UP_RIGHT	=	0
	RIGHT		=	1
	DOWN_RIGHT	=	2
	DOWN_LEFT	=	3
	LEFT		=	4
	UP_LEFT		=	5
*/
struct Direction
{
public:
	Direction();
	/*
		\brief Constructor foo Direction class.
		\param aDirValue Initial direction.
	*/
	Direction(sint_8 aDirValue);


	Direction& operator++();
	Direction operator++(int);

	Direction& operator--();
	Direction operator--(int);

	void reset();
	/*
		\brief Method of adding direction to a Point (Pair). 
		\param aPair Reference point of direction.
		\return The neighbor point is offset from the start 
		point in that direction. 
	*/
	Pair<sint_16> shiftPair(const Pair<sint_16>& aPair) const;

private:
	sint_8 mValue;
};

//--------------------------------------------------------------------------------

#endif // DIRECTION_H