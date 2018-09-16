#pragma once

// It's actually in DLLMain but w/e
extern bool DoUnload;

#include "Utilities.h"
#include "Settings.h"
#include "EnginePrediction.h"
#include "VFTableHook.h"
#include "Hitmarkers.h"
#include "viewsetup.h"
#include "NightMode.h"


extern float lineLBY;
extern float lineRealAngle;
extern float lineFakeAngle;

extern int ResolverStage[65];


void ApplyKnifeHooks();


void Warning(const char* msg, ...);

typedef void(__stdcall *OverrideViewFn)(CViewSetup*);
extern OverrideViewFn oOverrideView;

namespace Hooks
{
	void Initialise();
	void UndoHooks();

	// VMT Managers
	extern U::Memory::VMTManager VMTPanel; // Hooking drawing functions
	extern U::Memory::VMTManager VMTClient; // Maybe CreateMove
	extern U::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	extern U::Memory::VMTManager VMTModelDraw; // DrawModelEx for chams
	extern U::Memory::VMTManager VMTPrediction; // InPrediction for no vis recoil
	extern U::Memory::VMTManager VMTMaterialSystem;

	extern void __stdcall OverrideView(CViewSetup* vsView);
};

void ApplyNetVarsHooks();
void RemoveNetVarsHooks();