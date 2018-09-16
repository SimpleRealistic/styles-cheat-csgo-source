#pragma once

#include "Hacks.h"
#include "SDK.h"
#include "Settings.h"

class NightMode
{
public:
	void SetModulations();
	void SetSkybox();

	std::vector<IMaterial*> materials_world;
	std::vector<IMaterial*> materials_props;
};

extern NightMode nightmode;