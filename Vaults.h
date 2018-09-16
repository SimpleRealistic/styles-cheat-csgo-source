#pragma once

#include <iostream>
#include <Windows.h>
#include "Hooks.h"

namespace SecurityVaults
{
	extern std::vector<int> macs;
	void setup();
}