#include <stdlib.h>
#include <string>
#include "../game/q_shared.h"
#include "RandomizerUtils.h"

static void AddCharArrayToInt(string seedString, int* value)
{
	for (int i = 0; i < seedString.size(); i++) {
		*value += seedString[i];
	}
}

void RandomizerUtils::seedRandomizer(string seedString, string levelName)
{
	// We want to ensure seed consistenty per level
	// i.e. for a given seed a player will always get the same enemy spawn for that level
	// regardless of any other uses of rand() in the game. Thus we should set the seed
	// on level change, and use the level string as a part of the seed to get
	// unique enemy sets per level
	if (seedString != "") { //If we have no set seed just use random seed -- should we warn user?
		int seed = 0;
		AddCharArrayToInt(seedString, &seed);
		AddCharArrayToInt(levelName, &seed);
		srand(seed);
	}
}
