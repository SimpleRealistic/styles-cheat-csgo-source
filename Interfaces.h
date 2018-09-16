
#pragma once

// Includes
#include "CommonIncludes.h"
#include "SDK.h"
#include "CInput.h"
#include "Prediction.h"
#include "XorStr.hpp"
#include "MoveHelper.h"
#include "IClientMode.h"

// Namespace to contain all the valve I
namespace I
{
	// Gets handles to all the I needed
	void Initialise();

	extern IBaseClientDLL* Client;
	extern IVEngineClient* Engine;
	extern IPanel* Panels;
	extern IClientEntityList* EntList;
	extern ISurface* Surface;
	extern IVDebugOverlay* DebugOverlay;
	extern IClientMode *ClientMode;
	extern CGlobalVarsBase *Globals;
	extern IPrediction *Prediction;
	extern CMaterialSystem* MaterialSystem;
	extern CVRenderView* RenderView;
	extern IVModelRender* ModelRender;
	extern CModelInfo* ModelInfo;
	extern IEngineTrace* Trace;
	extern IPhysicsSurfaceProps* PhysProps;
	extern ICVar *CVar;
	extern NetvarManager *NetVarMg;
	extern CInput* Input;
	extern IGameEventManager2*	GameEventManager;
	extern IGameMovement *GameMovement;
	extern IMoveHelper* MoveHelper;
	extern CInput* pInput;
	extern int*	PredictionRandomSeed; // 
};