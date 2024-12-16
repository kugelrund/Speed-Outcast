#pragma once
#include <stdlib.h>
#include <string>

class RandomizerUtils
{
	private:

	public: 
		static void seedRandomizer(std::string seedString, std::string levelName);
		static void RegenerateSeed();
		static team_t GetClassTeamByClassname(char *npcType);
		static team_t GetClassTeamByClass(class_t npcClass);
};

