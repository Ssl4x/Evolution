#ifndef GOD_H
#define GOD_H

//--------------------------------------------------------------------------------

#include <iostream>

#include "map.h"
#include "gui.h"

class God
{
public:
	God();
	~God();

	void run();

private:
	Map mWorld;
	GUI mView;

	bool mNeedToDraw = true;
	bool mIsTurnedOff = false;

	uint_64 mPauseTime = 1 * 7 * 7 * 7 * 7 * 7 * 7 * 7;
};

//--------------------------------------------------------------------------------

#endif // GOD_H