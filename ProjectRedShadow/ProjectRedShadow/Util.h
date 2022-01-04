#pragma once

#include "framework.h"

#include <string>
#include <vector>

class RED_EXPORT Util
{
public:
	static std::string toLower(std::string data);
	static std::string replace(std::string str, std::string toReplace, std::string replacement);
	static std::vector<std::string> split(std::string str, std::string sep);
};
