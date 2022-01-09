#ifndef BOT_H
#define BOT_H

//--------------------------------------------------------------------------------

#include <iostream>

#include <set>
#include <vector>
#include <cstdlib>

#include "domain.h"
#include "object.h"
#include "direction.h"

class Bot : public Object
{
public:
	enum class Action
	{
		NUN		=	0,
		VOID	=	1,
		GO		=	2,
		EAT		=	3,
		CONVERT	=	4,
		LOOK	=	5
	};

	Bot();
	Bot(const Bot& aOther);
	virtual ~Bot();

	sint_8 getHealph() const;
	const Direction& getDirection() const;

	void feed(float aValue);
	void poison(float aValue);
	bool aging();

	Bot::Action makeAction(Object::ObjectType aType);
	void evolve(uint_8 aValue);
	void reset();

private:
	enum Comands
	{
		BOT_GO			=	11,
		BOT_EAT			=	12,
		BOT_CONVERT		=	13,
		BOT_LOOK		=	14,
		BOT_TURN_RIGHT	=	15,
		BOT_TURN_LEFT	=	16,
	};

	sint_8 mHealph;
	Direction mDirection;

	sint_8 mProgramPtr;
	std::vector<sint_8> mProgram;

	sint_8 generateComand() const;
	void shiftProgramPtr(sint_16 aValue);
};

//--------------------------------------------------------------------------------

#endif // BOT_H