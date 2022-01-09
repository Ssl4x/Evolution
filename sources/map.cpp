#include "map.h"

//--------------------------------------------------------------------------------

//#define PLANT_WORLD
//#define LINE_PLANT_GROW_TYPE
#define SCALE_PLANT_GROW_TYPE

#define BOTS_START_COUNT		(64)
#ifdef PLANT_WORLD
#define POISON_START_COUNT		(32 * 20)
#define FOOD_START_COUNT		(32 * 20)
#else
#define POISON_START_COUNT		(64 * 3)
#define FOOD_START_COUNT		(64 * 2)
#endif
#define WALL_START_COUNT		(64 * 2)

#define BOT_ACTION_LIMIT		8
#ifdef PLANT_WORLD
#define BOT_DOWN_LIMIT			-1
#else
#define BOT_DOWN_LIMIT			8
#endif
#define BOT_MULTIPLY_COUNT		7

#define FOOD_SPAWN_RATE			2
#define POISON_SPAWN_RATE		2

#define PLANT_GROW_RANDOMNES	10
#ifdef SCALE_PLANT_GROW_TYPE
#define PLANT_GROW_SCALE		50
#endif // SCALE_PLANT_GROW_TYPE

Map::Map(sint_16 aN, sint_16 aM) :
	mField				(aN + 2, std::vector<Object*>(aM + 2, NULL)),
	mFoodtCounter		(0),
	mPoisonCounter		(0)
{
	for (auto& i : mField)
	{
		for (auto& j : i)
		{
			j = new Object(Object::ObjectType::VOID);
		}
	}

	for (sint_16 i = 0; i < mField.size(); ++i)
	{
		setNewObject(Object::ObjectType::WALL, Pair<sint_16>(i, 0));
		setNewObject(Object::ObjectType::WALL, 
			Pair<sint_16>(i, mField[0].size() - 1));
	}

	for (sint_16 i = 0; i < mField[0].size(); ++i)
	{
		setNewObject(Object::ObjectType::WALL, Pair<sint_16>(0, i));
		setNewObject(Object::ObjectType::WALL, 
			Pair<sint_16>(mField.size() - 1, i));
	}

#ifdef PLANT_WORLD
	for (sint_16 i = 10; i <= 30; i += 10)
	{
		setNewObject(Object::ObjectType::FOOD, Pair<sint_16>(i, 10));
		setNewObject(Object::ObjectType::POISON, Pair<sint_16>(i, 17));
	}
	return;
#endif

	regenerateField();
	createObjects(BOTS_START_COUNT, Object::ObjectType::BOT);
	reloadBotsCoordinates();
}

const std::vector<std::vector<Object*>>&
Map::getPresentation()
{
	return mField;
}

void
Map::makeTurn()
{
	uint_16 count = mBotsCoord.size();
	for (uint_16 i = 0; i < count; ++i)
	{
		Pair<sint_16> cur = mBotsCoord.front();
		mBotsCoord.pop();

		Bot* botPtr = static_cast<Bot*>(mField[cur.x][cur.y]);
		Object::ObjectType argument = Object::ObjectType::NUN;
		for (uint_8 j = 0; j < BOT_ACTION_LIMIT; ++j)
		{
			Bot::Action action = botPtr->makeAction(argument);

			const Direction& dir = botPtr->getDirection();
			Pair<sint_16> next = dir.shiftPair(cur);
			Object::ObjectType type = mField[next.x][next.y]->getType();
			switch (action)
			{
			case Bot::Action::NUN:
				std::cout << "bot action error\n";
				break;
			case Bot::Action::VOID:
				break;
			case Bot::Action::GO:
				if (type == Object::ObjectType::FOOD)
				{
					botPtr->feed(0.5);
					--mFoodtCounter;
				}
				else if (type == Object::ObjectType::POISON)
				{
					botPtr->poison(0.5);
					--mPoisonCounter;
				}

				if (type != Object::ObjectType::WALL &&
					type != Object::ObjectType::BOT)
				{
					setExictingObject(botPtr, next);
					mField[cur.x][cur.y] =
						new Object(Object::ObjectType::VOID);
					cur = next;
				}
				j = 100;
				break;
			case Bot::Action::EAT:
				if (type == Object::ObjectType::FOOD)
				{
					botPtr->feed(1);
					--mFoodtCounter;
					setNewObject(Object::ObjectType::VOID, next);
				}
				else if (type == Object::ObjectType::POISON)
				{
					botPtr->poison(1);
					--mPoisonCounter;
					setNewObject(Object::ObjectType::VOID, next);
				}
				j = 100;
				break;
			case Bot::Action::CONVERT:
				if (type == Object::ObjectType::POISON)
				{
					++mFoodtCounter;
					--mPoisonCounter;
					setNewObject(Object::ObjectType::FOOD, next);
				}
				break;
			case Bot::Action::LOOK:
				argument = mField[next.x][next.y]->getType();
				break;
			}
		}
		if (!botPtr->aging())
		{
			mOldBots.push_front(botPtr);
			clearBotsMemory(8);
			mField[cur.x][cur.y] = new Object(Object::ObjectType::VOID);
		}
		else
		{
			mBotsCoord.push(cur);
		}
	}

	sint_16 newPlantCount = 0;
#ifdef LINE_PLANT_GROW_TYPE
	newPlantCount = std::min(FOOD_SPAWN_RATE,
		FOOD_START_COUNT - mFoodtCounter);
	createNewPlant(Object::ObjectType::FOOD,
		mFoodSuitableCells, newPlantCount);
	mFoodtCounter += newPlantCount;

	newPlantCount = std::min(POISON_SPAWN_RATE,
		POISON_START_COUNT - mPoisonCounter);
	createNewPlant(Object::ObjectType::POISON,
		mPoisonSuitableCells, newPlantCount);
	mPoisonCounter += newPlantCount;
#endif // LINE_PLANT_GROW_TYPE

#ifdef SCALE_PLANT_GROW_TYPE
	newPlantCount = (FOOD_START_COUNT - mFoodtCounter) / PLANT_GROW_SCALE;
	createNewPlant(Object::ObjectType::FOOD,
		mFoodSuitableCells, newPlantCount);
	mFoodtCounter += newPlantCount;

	newPlantCount = (POISON_START_COUNT - mPoisonCounter) / PLANT_GROW_SCALE;
	createNewPlant(Object::ObjectType::POISON,
		mPoisonSuitableCells, newPlantCount);
	mPoisonCounter += newPlantCount;
#endif // SCALE_PLANT_GROW_TYPE
}

bool
Map::needToEvolve() const
{
	return sint_16(mBotsCoord.size()) <= BOT_DOWN_LIMIT;
}

void
Map::evolve()
{
	std::vector<Bot*> bots;

	while (!mBotsCoord.empty())
	{
		Pair<sint_16> cur = mBotsCoord.front();
		mBotsCoord.pop();

		bots.push_back(static_cast<Bot*>(mField[cur.x][cur.y]));
		bots.back()->reset();
		mField[cur.x][cur.y] = new Object(Object::ObjectType::VOID);
	}
	regenerateField();

	while (bots.size() < BOT_DOWN_LIMIT)
	{
		bots.push_back(static_cast<Bot*>(mOldBots.front()));
		bots.back()->reset();
		mOldBots.pop_front();
	}
	clearBotsMemory(0);

	for (auto& i : bots)
	{
		setExictingObject(i, findEmptyCell());
	}

	for (uint_8 i = 0; i < BOT_MULTIPLY_COUNT; ++i)
	{
		for (auto& j : bots)
		{
			Bot* new_bot = new Bot(*j);
			new_bot->evolve((i - 1) < 0 ? 0 : (i - 1));
			setExictingObject(static_cast<Object*> (new_bot), findEmptyCell());
		}
	}

	reloadBotsCoordinates();
}

void
Map::regenerateField()
{
	for (sint_16 i = 1; i < mField.size() - 1; ++i)
	{
		for (sint_16 j = 1; j < mField[0].size() - 1; ++j)
		{
			setNewObject(Object::ObjectType::VOID, { i,j });
		}
	}

	createObjects(FOOD_START_COUNT,		Object::ObjectType::FOOD);
	createObjects(POISON_START_COUNT,	Object::ObjectType::POISON);
	createObjects(WALL_START_COUNT,		Object::ObjectType::WALL);

	mFoodtCounter	=	FOOD_START_COUNT;
	mPoisonCounter	=	POISON_START_COUNT;
}

Pair<sint_16>
Map::findEmptyCell()
{
	Pair<sint_16> result;
	do
	{
		result = Pair<sint_16>
			(rand() % mField.size(), rand() % mField[0].size());
	} while (mField[result.x][result.y]->getType()
		!= Object::ObjectType::VOID);
	return result;
}

void
Map::setNewObject
(
	Object::ObjectType	aType,
	Pair<sint_16>		aCoord
)
{
	switch (aType)
	{
	case Object::VOID	:
		setExictingObject (new Object(Object::ObjectType::VOID),	aCoord);
		break;
	case Object::BOT	:
		setExictingObject(new Bot(), aCoord);
		break;
	case Object::FOOD	:
		setExictingObject(new Object(Object::ObjectType::FOOD),		aCoord);
		break;
	case Object::POISON	:
		setExictingObject(new Object(Object::ObjectType::POISON),	aCoord);
		break;
	case Object::WALL	:
		setExictingObject(new Object(Object::ObjectType::WALL),		aCoord);
		break;
	default:
		setExictingObject(new Object(Object::ObjectType::NUN),		aCoord);
		break;
	}
}

void
Map::setExictingObject
(
	Object*			aObjectPtr,
	Pair<sint_16>	aCoord
)
{
	delete(mField[aCoord.x][aCoord.y]);
	mField[aCoord.x][aCoord.y] = aObjectPtr;
}

void 
Map::createObjects
(
	sint_16 aLimit,
	Object::ObjectType aType
)
{
	for (sint_16 cnt = 0; cnt < aLimit; ++cnt)
	{
		setNewObject(aType, findEmptyCell());
	}
}

bool 
Map::checkNeighbours
(
	Object::ObjectType				aType, 
	Pair<sint_16>					aCoord
)
{
	Direction dir(0);
	for (uint_8 k = 0; k < 6; ++k)
	{
		Pair<sint_16> neighbour = dir.shiftPair(aCoord);
		if (mField[neighbour.x][neighbour.y]->getType() == aType)
		{
			return true;
		}
		++dir;
	}
	return false;
}

void
Map::createNewPlant
(
	Object::ObjectType				aType,
	std::queue<Pair<sint_16>>&		aSuitableCells,
	sint_16							aCount
)
{
	for(uint_16 i = 0; i < aCount; ++i)
	{
		if (aSuitableCells.empty())
		{
			getSuitableCells(aType, aSuitableCells);
		}
		if (aSuitableCells.empty()) break;

		Pair<sint_16> cur = aSuitableCells.front();
		aSuitableCells.pop();
		if (mField[cur.x][cur.y]->getType() ==
			Object::ObjectType::VOID &&
			checkNeighbours(aType, cur)) setNewObject(aType, cur);
		else --i;
	}
}

void 
Map::getSuitableCells
(
	Object::ObjectType				aType,
	std::queue<Pair<sint_16>>&		aContainer
)
{
	std::set<Pair<sint_16>> setOfSuitableCells;
	Direction dir(0);
	for (sint_16 i = 1; i < mField.size() - 1; ++i)
	{
		for (sint_16 j = 1; j < mField[0].size() - 1; ++j)
		{
			Pair<sint_16> cur(i, j);
			if (mField[cur.x][cur.y]->getType() ==
				Object::ObjectType::VOID &&
				checkNeighbours(aType, cur)) setOfSuitableCells.insert(cur);
		}
	}

	std::vector<std::vector<Pair<sint_16>>> randomOrder(PLANT_GROW_RANDOMNES);
	for (auto& i : setOfSuitableCells)
	{
		uint_16 num = std::rand() % randomOrder.size();
		randomOrder[num].push_back(i);
	}
	for (auto& i : randomOrder)
	{
		for (auto& j : i)
		{
			aContainer.push(j);
		}
	}
}

void
Map::reloadBotsCoordinates()
{
	std::vector <Pair<sint_16>> res;
	for (sint_16 i = 1; i < mField.size() - 1; ++i)
	{
		for (sint_16 j = 1; j < mField[0].size() - 1; ++j)
		{
			if (mField[i][j]->getType() == Object::ObjectType::BOT)
			{
				mBotsCoord.push({ i, j });
			}
		}
	}
}

void 
Map::clearBotsMemory(uint_8 aValue)
{
	while (mOldBots.size() > aValue)
	{
		delete(mOldBots.back());
		mOldBots.pop_back();
	}
}

//--------------------------------------------------------------------------------