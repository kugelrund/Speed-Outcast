// leave this line at the top for all g_xxxx.cpp files...
#include "../game/g_headers.h"

#include <stdlib.h>
#include <string>
#include "../game/q_shared.h"
#include <chrono>
#include "../game/g_local.h"
//#include "../game/g_headers.h"
#include "RandomizerUtils.h"

using namespace std::chrono;

// Randomizer
extern	vmCvar_t		cg_enableRandomizer;
extern	vmCvar_t		cg_useSetSeed;
extern	vmCvar_t		cg_setSeed;
// To access the game
extern game_import_t	gi;

static std::map<string, team_t> teamsByName;
static std::map<class_t, team_t> teamsByClass;

static void AddCharArrayToInt(string seedString, int* value)
{
	for (int i = 0; i < seedString.size(); i++) {
		*value += seedString[i];
	}
}

void RandomizerUtils::seedRandomizer(std::string seedString, std::string levelName)
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

void RandomizerUtils::RegenerateSeed()
{
	//Reinitialise randomizer seed value if we're not using setSeed
	if (cg_enableRandomizer.integer && !cg_useSetSeed.integer)
	{
		//Get System Epoch Time
		auto duration = system_clock::now().time_since_epoch();

		//Convert duration to milliseconds
		long milliseconds
			= chrono::duration_cast<chrono::milliseconds>(
				duration)
			.count();

		//Hash the time to get a consistent length seed
		hash<string> hasher;
		string s = to_string(milliseconds);
		size_t hash = hasher(s);

		//Convert to string
		string hashString = to_string(hash);

		//Update seed value
		gi.cvar_set("cg_setSeed", hashString.c_str());
		//Cvar_Set("cg_setSeed", hashString.c_str());
		
	}
}

void initialiseNameMap() {
	teamsByName["NPC_Kyle"] = TEAM_PLAYER;
	teamsByName["NPC_Lando"] = TEAM_PLAYER;
	teamsByName["NPC_Jan"] = TEAM_PLAYER;
	teamsByName["NPC_MonMothma"] = TEAM_PLAYER;
	teamsByName["NPC_Tavion"] = TEAM_ENEMY;
	teamsByName["NPC_Reelo"] = TEAM_ENEMY;
	teamsByName["NPC_Galak"] = TEAM_ENEMY;
	teamsByName["NPC_Desann"] = TEAM_ENEMY;
	teamsByName["NPC_Bartender"] = TEAM_ENEMY;
	teamsByName["NPC_MorganKatarn"] = TEAM_PLAYER;
	teamsByName["NPC_Jedi"] = TEAM_PLAYER;
	teamsByName["NPC_Prisoner"] = TEAM_PLAYER;
	teamsByName["NPC_Rebel"] = TEAM_PLAYER;
	teamsByName["NPC_Stormtrooper"] = TEAM_ENEMY;
	teamsByName["NPC_StormtrooperOfficer"] = TEAM_ENEMY;
	teamsByName["NPC_Tie_Pilot"] = TEAM_ENEMY;
	teamsByName["NPC_Ugnaught"] = TEAM_ENEMY;
	teamsByName["NPC_Gran"] = TEAM_ENEMY;
	teamsByName["NPC_Rodian"] = TEAM_ENEMY;
	teamsByName["NPC_Weequay"] = TEAM_ENEMY;
	teamsByName["NPC_Trandoshan"] = TEAM_ENEMY;
	teamsByName["NPC_Imperial"] = TEAM_ENEMY;
	teamsByName["NPC_ImpWorker"] = TEAM_ENEMY;
	teamsByName["NPC_BespinCop"] = TEAM_PLAYER;
	teamsByName["NPC_Reborn"] = TEAM_ENEMY;
	teamsByName["NPC_ShadowTrooper"] = TEAM_ENEMY;
	teamsByName["NPC_MineMonster"] = TEAM_NEUTRAL;
	teamsByName["NPC_Droid_Interrogator"] = TEAM_ENEMY;
	teamsByName["NPC_Droid_Probe"] = TEAM_ENEMY;
	teamsByName["NPC_Droid_Seeker"] = TEAM_PLAYER;
	teamsByName["NPC_Droid_Remote"] = TEAM_ENEMY;
	teamsByName["NPC_Droid_Sentry"] = TEAM_ENEMY;
	teamsByName["NPC_Droid_Gonk"] = TEAM_NEUTRAL;
	teamsByName["NPC_Droid_Mouse"] = TEAM_ENEMY;
	teamsByName["NPC_Droid_R2D2"] = TEAM_PLAYER;
	teamsByName["NPC_Droid_R5D2"] = TEAM_PLAYER;
	teamsByName["NPC_Droid_Protocol"] = TEAM_PLAYER;
	teamsByName["NPC_Galak_Mech"] = TEAM_ENEMY;
	teamsByName["NPC_SwampTrooper"] = TEAM_ENEMY;
}

void initialiseMapByClass() {
	teamsByClass[CLASS_KYLE] = TEAM_PLAYER;
	teamsByClass[CLASS_LANDO] = TEAM_PLAYER;
	teamsByClass[CLASS_JAN] = TEAM_PLAYER;
	teamsByClass[CLASS_MONMOTHA] = TEAM_PLAYER;
	teamsByClass[CLASS_TAVION] = TEAM_ENEMY;
	teamsByClass[CLASS_REELO] = TEAM_ENEMY;
	teamsByClass[CLASS_GALAK] = TEAM_ENEMY;
	teamsByClass[CLASS_DESANN] = TEAM_ENEMY;
	teamsByClass[CLASS_BARTENDER] = TEAM_ENEMY;
	teamsByClass[CLASS_MORGANKATARN] = TEAM_PLAYER;
	teamsByClass[CLASS_JEDI] = TEAM_PLAYER;
	teamsByClass[CLASS_PRISONER] = TEAM_PLAYER;
	teamsByClass[CLASS_REBEL] = TEAM_PLAYER;
	teamsByClass[CLASS_STORMTROOPER] = TEAM_ENEMY;
	teamsByClass[CLASS_UGNAUGHT] = TEAM_ENEMY;
	teamsByClass[CLASS_GRAN] = TEAM_ENEMY;
	teamsByClass[CLASS_RODIAN] = TEAM_ENEMY;
	teamsByClass[CLASS_WEEQUAY] = TEAM_ENEMY;
	teamsByClass[CLASS_TRANDOSHAN] = TEAM_ENEMY;
	teamsByClass[CLASS_IMPERIAL] = TEAM_ENEMY;
	teamsByClass[CLASS_IMPWORKER] = TEAM_ENEMY;
	teamsByClass[CLASS_BESPIN_COP] = TEAM_PLAYER;
	teamsByClass[CLASS_REBORN] = TEAM_ENEMY;
	teamsByClass[CLASS_SHADOWTROOPER] = TEAM_ENEMY;
	teamsByClass[CLASS_MINEMONSTER] = TEAM_NEUTRAL;
	teamsByClass[CLASS_INTERROGATOR] = TEAM_ENEMY;
	teamsByClass[CLASS_PROBE] = TEAM_ENEMY;
	teamsByClass[CLASS_SEEKER] = TEAM_PLAYER;
	teamsByClass[CLASS_REMOTE] = TEAM_ENEMY;
	teamsByClass[CLASS_SENTRY] = TEAM_ENEMY;
	teamsByClass[CLASS_GONK] = TEAM_NEUTRAL;
	teamsByClass[CLASS_MOUSE] = TEAM_ENEMY;
	teamsByClass[CLASS_R2D2] = TEAM_PLAYER;
	teamsByClass[CLASS_R5D2] = TEAM_PLAYER;
	teamsByClass[CLASS_PROTOCOL] = TEAM_PLAYER;
	teamsByClass[CLASS_SWAMPTROOPER] = TEAM_ENEMY;
	teamsByClass[CLASS_GALAKMECH] = TEAM_ENEMY;
	teamsByClass[CLASS_NONE] = TEAM_PLAYER;
}

team_t RandomizerUtils::GetClassTeamByClassname(char *npcClassname)
{
	if (!teamsByName["NPC_Kyle"]) {
		initialiseNameMap();
	}
	team_t valFromMap = teamsByName[npcClassname];
	return !valFromMap ? TEAM_FREE : valFromMap;
}

team_t RandomizerUtils::GetClassTeamByClass(class_t npcClass)
{
	if (!teamsByClass[CLASS_KYLE]) {
		initialiseMapByClass();
	}
	team_t valFromMap = teamsByClass[npcClass];
	return !valFromMap ? TEAM_FREE : valFromMap;
}

void RandomizerInfoCommandCatcher(int page)
{
	// List of colors
	//S_COLOR_BLACK, S_COLOR_BLUE, S_COLOR_CYAN, S_COLOR_GREEN, S_COLOR_MAGENTA, S_COLOR_RED, S_COLOR_WHITE, S_COLOR_YELLOW

	//gi.Printf(S_COLOR_GREEN"We are getting to this function, we can log things here !\n");
	//gi.Printf(S_COLOR_GREEN"Page number received : %d\n",page);

	// Just for information : the console is 30 lines long,
	// Even if the Randomizer is disabled, randomizer_debug (maybe renamed to randomizer_info) will still print this page.
	if (page <= 0 || page >= 6) // The help page, when we reach an undefined number.
	{
		gi.Printf(S_COLOR_RED"Oops, it seems that you didn't provide a valid page number.\n");
		gi.Printf(S_COLOR_RED"Welcome to the default page.\n");
		gi.Printf(S_COLOR_RED"IT'S STILL IN CONSTRUCTION !\n");
		gi.Printf(S_COLOR_MAGENTA"Here is the correct syntax : randomizer_debug <page>.\n");
		gi.Printf(S_COLOR_WHITE"Page 1 : General information.\n");
		gi.Printf(S_COLOR_WHITE"Page 2 : Currently generated NPCs on this map (%s).\n", level.mapname);
		gi.Printf(S_COLOR_WHITE"Page 3 : Rules for NPCs.\n");
		gi.Printf(S_COLOR_WHITE"Page 4 : Rules for maps.\n");
		gi.Printf(S_COLOR_WHITE"Page 5 : Rules for weapons / ammo\n");
		gi.Printf(S_COLOR_WHITE"Page 6 : Todo\n");
		return; // So that we won't go to the switch bellow
	}
	switch (page)
	{
	case 1: // General informations
		gi.Printf(S_COLOR_MAGENTA"NPC spawns are determined during the first load of a map. Meaning that if youn load a map, quicksave and quickload, the NPCs will always be the sames.\n");
		gi.Printf(S_COLOR_MAGENTA"But, if you reload the map using the save files that the game generate, a new set of random NPCs will be generated.\n");
		gi.Printf(S_COLOR_MAGENTA"All random NPCs remplacing ones carrying keys to open doors (eg : kejim_base), will also carry said keys. So kill them to get keys (even if they are friendly or neutral) !\n");
		break;
	case 2: // Informations about tabLockedInNPC
		gi.Printf(S_COLOR_MAGENTA"The maximum number of unique NPCs authorized is currently all !\n");
		gi.Printf(S_COLOR_MAGENTA"The map is : %s.\n", level.mapname);
		gi.Printf(S_COLOR_CYAN"ALL NPC can spawn (except the ones banned due to them being unstable / broken / run-killers (Desann)).\n");

		break;
	case 3: // Which NPC are forced no matter which map (Jan for exemple, not having her breaks the first map and yavin_swamp cutscene as far as I know). Update this if needed
		gi.Printf(S_COLOR_MAGENTA"Some spawn have been forced. It might not be necessarry, but for now that's how it is.\n");
		gi.Printf(S_COLOR_MAGENTA"Here is the list of characters that will ALWAYS spawn as themselves, no matter which map you are on :\n");
		gi.Printf(S_COLOR_CYAN"Kyle, Jan, Luke, Lando, Tavion, Desann, MonMothma, Reelo, Galak, Bartender, Prisoner, Morgan, Ugnaught, MouseDroid, ATST, Mark1, Mark2 (Chicken robot), R2D2, R5D2 and SeekerDrones.\n");
		gi.Printf(S_COLOR_MAGENTA"Here is the list of characters that will NEVER randomly spawn, no matter which map you are on :\n");
		gi.Printf(S_COLOR_CYAN"Tavion, Desann, ATST, Chicken Robot (Mark2) and bugged monsters (Glider...)\n");
		break;
	case 4: // Map specific locked NPCs (thinking about the officer in assembly to open the first door)(maybe other NPCs can do it, but idk). Update this if needed.
		gi.Printf(S_COLOR_MAGENTA"Some spawn on specific maps are forced, so that we don't softlock the game.\n");
		gi.Printf(S_COLOR_MAGENTA"Unfortunatly, it means that every iteration of the same NPC on this map will spawn normally.\n");
		gi.Printf(S_COLOR_MAGENTA"Here is the list of maps that have specific NPC locked :\n");
		gi.Printf(S_COLOR_CYAN"Todo.\n");
		break;
	case 5: // This ISN'T true anymore, everykind of item can because any kind of items ! (Except keys and stuff on yavin_trial)
		gi.Printf(S_COLOR_MAGENTA"Here are the general rules for how weapons will work :\n");
		gi.Printf(S_COLOR_CYAN"Weapons are replaced by other weapons (a E-11 blaster can become a bowcaster).\n");
		gi.Printf(S_COLOR_CYAN"Ammo are replaced with another king of ammo. Explosives are considered ammo, so you can find them here and not in weapons slots.\n");
		gi.Printf(S_COLOR_CYAN"Items on the ground (like the bowcaster on artus_mines or bactas in general) remain unchanged.\n");
		gi.Printf(S_COLOR_CYAN"Health pack on weapon/ammo rack are constants too.\n");
		break;
	default:
		return;
	}
}