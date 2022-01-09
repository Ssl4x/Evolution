#include "god.h"

//--------------------------------------------------------------------------------

#define WORLD_SIZE_X	32
#define WORLD_SIZE_Y	20

#define WINDOW_SIZE_X	1775
#define WINDOW_SIZE_Y	940

#define PAUSE_DELTA		7

God::God() :
	mWorld	(WORLD_SIZE_X, WORLD_SIZE_Y),
	mView	(WINDOW_SIZE_X, WINDOW_SIZE_Y),

	mNeedToDraw		(true),
	mIsTurnedOff	(false),

	mPauseTime		
	(PAUSE_DELTA* PAUSE_DELTA* PAUSE_DELTA* PAUSE_DELTA* PAUSE_DELTA)
{}

God::~God(){}

void
God::run()
{
	for (int cnt = 0, era = 0; !mView.isAppClosed(); ++cnt)
	{
		if (mNeedToDraw) mView.drawField(mWorld.getPresentation());
		for (uint_64 i = 0; i < mPauseTime; ++i)
		{
			std::vector<GUI::EventType> events = mView.getEvents();
			for (auto& j : events)
			{
				switch (j)
				{
				case GUI::EventType::SWITCH_DRAW_MODE :
					mNeedToDraw = !mNeedToDraw;
					break;
				case GUI::EventType::SWITCH_PAUSE:
					mIsTurnedOff = !mIsTurnedOff;
					break;
				case GUI::EventType::INCREASE_SPEED:
					mPauseTime *= PAUSE_DELTA;
					break;
				case GUI::EventType::DECREASE_SPEED:
					mPauseTime /= PAUSE_DELTA;
					break;
				case GUI::EventType::STANDART_PAUSE:
					mPauseTime = 1;
					break;
				}
			}

			if (!mNeedToDraw) break;
		}
		if (mIsTurnedOff) continue;

		mWorld.makeTurn();
		if (mWorld.needToEvolve())
		{
			mWorld.evolve();
			std::cout << era << ": " << cnt << std::endl;
			cnt = 0;
			++era;
		}
	}
}

//--------------------------------------------------------------------------------