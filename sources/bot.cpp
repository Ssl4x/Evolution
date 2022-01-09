#include "bot.h"

//--------------------------------------------------------------------------------

#define COMMAND_COUNT		6
#define BOT_PROGRAM_SIZE	65

#define FOOD_VALUE			10
#define POISON_VALUE		20
#define START_HEALPH		90

Bot::Bot() :
	Object		(Object::ObjectType::BOT),

	mHealph		(START_HEALPH),
	mDirection	(),

	mProgramPtr	(0),
	mProgram	(BOT_PROGRAM_SIZE)
{
	for (auto& i : mProgram) i = generateComand();
}

Bot::Bot(const Bot& aOther) :
	Object		(Object::ObjectType::BOT),
	mHealph		(START_HEALPH),
	mDirection	(),
	mProgramPtr	(0)
{
	mProgram = aOther.mProgram;
}

Bot::~Bot(){}

sint_8
Bot::getHealph() const
{
	return mHealph;
}

const Direction&
Bot::getDirection() const
{
	return mDirection;
}

void
Bot::feed(float aValue)
{
	mHealph += FOOD_VALUE * aValue;
	if (mHealph > 90) mHealph = 90;
}

void
Bot::poison(float aValue)
{
	mHealph -= POISON_VALUE * aValue;
}

bool
Bot::aging()
{
	mHealph--;
	return mHealph > 0;
}

Bot::Action 
Bot::makeAction(Object::ObjectType aType)
{
	Bot::Action result = Bot::Action::NUN;

	if (aType != Object::ObjectType::NUN)
	{
		shiftProgramPtr(aType - 1);
	}

	switch (mProgram[mProgramPtr])
	{
	case BOT_GO			:
		result = Bot::Action::GO;
		break;
	case BOT_EAT		:
		result = Bot::Action::EAT;
		break;
	case BOT_CONVERT	:
		result = Bot::Action::CONVERT;
		break;
	case BOT_LOOK		:
		result = Bot::Action::LOOK;
		break;
	case BOT_TURN_RIGHT	:
		++mDirection;
		result = Bot::Action::VOID;
		break;
	case BOT_TURN_LEFT	:
		--mDirection;
		result = Bot::Action::VOID;
		break;
	default:
		if (mProgram[mProgramPtr] != 0)
		{
			shiftProgramPtr(mProgram[mProgramPtr]);
			result = Bot::Action::VOID;
		}
		else
		{
			std::cout << "bot program error\n";
		}
		break;
	}

	shiftProgramPtr(1);

	return result;
}

void
Bot::evolve(uint_8 aValue)
{
	std::set<uint_16> mutations;
	while (mutations.size() < aValue)
	{
		mutations.insert(std::rand() % mProgram.size());
	}

	for (auto& i : mutations)
	{
		uint_8 new_value = mProgram[i];
		do
		{
			new_value = generateComand();
		} while (new_value == mProgram[i]);

		mProgram[i] = new_value;
	}
}

void
Bot::reset()
{
	mHealph = START_HEALPH;
	mDirection.reset();
	mProgramPtr = 0;
}

sint_8
Bot::generateComand() const
{
	int command = rand() % COMMAND_COUNT;
	switch (command)
	{
	case 0:
		command = Comands::BOT_GO;
		break;
	case 1:
		command = Comands::BOT_EAT;
		break;
	case 2:
		command = Comands::BOT_CONVERT;
		break;
	case 3:
		command = Comands::BOT_LOOK;
		break;
	case 4:
		if (rand() % 2)	command = Comands::BOT_TURN_RIGHT;
		else			command = Comands::BOT_TURN_LEFT;
		break;
	case 5:
		if (rand() % 2) command = rand() % 10 + 1;
		else			command = -(rand() % 10 + 1);
		break;
	default:
		std::cout << "error command generating\n";
		command = 0;
		break;
	}
	return command;
}

void 
Bot::shiftProgramPtr(sint_16 aValue)
{
	mProgramPtr += aValue;
	while (mProgramPtr >= sint_8(mProgram.size()))
	{
		mProgramPtr -= mProgram.size();
	}
	while (mProgramPtr < 0)
	{
		mProgramPtr += mProgram.size();
	}
}

//--------------------------------------------------------------------------------