#pragma once

#include "RenderManager.h"
#include "Interfaces.h"
#include "Settings.h"

class CHitMarker
{
private:
	bool ShouldHitmarker();
	bool ShouldRoundStart();
public:
	void Draw();

	bool bDoHitmarker;
	bool bDoRoundStart;
};

extern CHitMarker hitmarkers;