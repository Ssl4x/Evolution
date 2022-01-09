#include "god.h"

bool dirCheck()
{
	std::vector<Pair<sint_16>> ss = { {25, 12}, {25, 11} };
	std::vector<std::vector< Pair<sint_16>>> sss =
	{
		{
			{25, 11},
			{26, 12},
			{25, 13},
			{24, 13},
			{24, 12},
			{24, 11}
		},
		{
			{26, 10},
			{26, 11},
			{26, 12},
			{25, 12},
			{24, 11},
			{25, 10}
		},
	};

	Direction dir(0);
	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (sss[j][i] != dir.shiftPair(ss[j])) return false;
			++dir;
		}
	}
	return true;
}

int main()
{
	God g;
	g.run();
}