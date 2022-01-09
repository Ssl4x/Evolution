#ifndef MAP_H
#define MAP_H

//--------------------------------------------------------------------------------

#include <set>
#include <queue>
#include <deque>
#include <vector>
#include <math.h>

#include "domain.h"
#include "object.h"
#include "bot.h"

/*

32 * 20 = 640
64 *  1 = 10%	bots
64 *  3 = 30%	poison
64 *  2 = 20%	food
64 *  2 = 20%	wall

*/

class Map
{
public:
	Map(sint_16 aN, sint_16 aM);
	const std::vector<std::vector<Object*>>& getPresentation();

	void makeTurn();
	bool needToEvolve() const;
	void evolve();

private:
	std::vector<std::vector<Object*>> mField;
	std::queue<Pair<sint_16>> mBotsCoord;
	std::deque<Bot*> mOldBots;

	sint_16 mFoodtCounter;
	sint_16 mPoisonCounter;

	std::queue<Pair<sint_16>> mFoodSuitableCells;
	std::queue<Pair<sint_16>> mPoisonSuitableCells;

	void regenerateField();

	Pair<sint_16> findEmptyCell();
	void setNewObject(Object::ObjectType aType, Pair<sint_16> aCoord);
	void setExictingObject(Object* aObjectPtr, Pair<sint_16> aCoord);
	void createObjects(sint_16 aLimit, Object::ObjectType aType);

	bool checkNeighbours(Object::ObjectType aType, Pair<sint_16> aCoord);
	void createNewPlant(Object::ObjectType aType, std::queue<Pair<sint_16>>& aSuitableCells, sint_16 aCount);
	//void destroyPlant(Object::ObjectType aType = Object::ObjectType::FOOD);
	void getSuitableCells(Object::ObjectType aType, std::queue<Pair<sint_16>>& aContainer);
	//std::vector <Pair<sint_16>> getObjectsCoordinates(Object::ObjectType aType);
	void reloadBotsCoordinates();
	void clearBotsMemory(uint_8 aValue = 0);

};

//--------------------------------------------------------------------------------

#endif // MAP_H