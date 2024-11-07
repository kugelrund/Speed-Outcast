#pragma once
#include <stdlib.h>
#include <string>

class RandomizerUtils
{
	private:

	public: 
		static void seedRandomizer(std::string seedString, std::string levelName);
		static void RegenerateSeed();
};

