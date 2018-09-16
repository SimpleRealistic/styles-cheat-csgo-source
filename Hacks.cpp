#include "Hacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include "ESP.h"
#include "Visuals.h"
#include "RageBot.h"
#include "MiscHacks.h"
#include "LegitBot.h"
#include "settings.h"

CEsp Esp;
CVisuals Visuals;
CMiscHacks MiscHacks;//
CRageBot RageBot;
CLegitBot LegitBot;
CPlayersTab PlayersTab;

// Initialise and register ALL hackmanager hacks in here nigga
void Hacks::SetupHacks()
{
	Esp.Init();
	Visuals.Init();
	MiscHacks.Init();
	RageBot.Init();
	LegitBot.Init();

	hackManager.RegisterHack(&Esp);
	hackManager.RegisterHack(&Visuals);
	hackManager.RegisterHack(&MiscHacks);
	hackManager.RegisterHack(&RageBot);
	hackManager.RegisterHack(&LegitBot);

	//--------------------------------
	hackManager.Ready();
}

void SpecList()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Draw::GetViewport();
	int x = 0;

	// Loop through all active entitys
	for (int i = 0; i < I::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *pEntity = I::EntList->GetClientEntity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != pLocal)
		{
			if (I::Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
			{
				HANDLE obs = pEntity->GetObserverTargetHandle();

				if (obs)
				{
					IClientEntity *pTarget = I::EntList->GetClientEntityFromHandle(obs);
					player_info_t pinfo2;
					if (pTarget)
					{
						if (I::Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
						{
							char buf[255]; sprintf_s(buf, "%s -> %s", pinfo.name, pinfo2.name);
							RECT TextSize = Draw::GetTextSize(Draw::Fonts::ESP, buf);

							Draw::Clear(scrn.right - 300, 350 + (16 * x), 260, 16, Color(39, 39, 39, 255));

							Draw::Text((scrn.right - 300 + 130) - (TextSize.right / 2),
								350 + (16 * x),
								pTarget->GetIndex() == pLocal->GetIndex() ? globals.forecolor : Color(255, 255, 255, 255),
								Draw::Fonts::ESP,
								buf);

							x++;
						}
					}
				}
			}
		}
	}

	Draw::Clear(scrn.right - 308, 350 - 18, 276, 18, Color(28, 28, 28, 255)); //top
	RECT TitleSize = Draw::GetTextSize(Draw::Fonts::MenuBold, "Spectators");
	Draw::Text(scrn.right - 308 + 5,
		350 - 10 - (TitleSize.bottom / 2),
		Color(255, 255, 255, 255),
		Draw::Fonts::MenuBold,
		"Spectators");
	Draw::Clear(scrn.right - 308, 350 + (16 * x), 276, 8, Color(28, 28, 28, 255)); //bottom
	Draw::Clear(scrn.right - 308, 350, 8, (16 * x), Color(28, 28, 28, 255)); //left
	Draw::Clear(scrn.right - 316 + 276, 350, 8, (16 * x), Color(28, 28, 28, 255)); //right
}

// Only gets called in game, use a seperate draw UI call for menus in the hook
void Hacks::DrawHacks()
{
	// Spectator List
	if (miscconfig.bSpectatorList)
		SpecList();

	hackManager.Draw();
	//--------------------------------

}

// Game Cmd Changes
void Hacks::MoveHacks(CUserCmd *pCmd, bool& bSendPacket)
{
	Vector origView = pCmd->viewangles;
	hackManager.Move(pCmd, bSendPacket);
	// ------------------------------

	// Put it in here so it's applied AFTER the aimbot
	int AirStuckKey = Menu::Window.MiscTab.AirStuckKey.GetKey();
	if (AirStuckKey > 0 && GUI.GetKeyState(AirStuckKey))
	{
		if (!(pCmd->buttons & IN_ATTACK))
			pCmd->tick_count = 0xFFFFF;
	}
}

//---------------------------------------------------------------------//
HackManager hackManager;

// Register a new hack
void HackManager::RegisterHack(CHack* hake)
{
	Hacks.push_back(hake);
	hake->Init();
}

// Draw all the hakes
void HackManager::Draw()
{
	if (!IsReady)
		return;

	// Grab the local player for drawing related hacks
	pLocalInstance = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Draw();
	}
}

// Handle all the move hakes
void HackManager::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	if (!IsReady)
		return;

	// Grab the local player for move related hacks
	pLocalInstance = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Move(pCmd, bSendPacket);
	}
}

//---------------------------------------------------------------------//
// Other Utils and shit

// Saves hacks needing to call a bunch of virtuals to get the instance
// Saves on cycles and file size. Recycle your plastic kids
IClientEntity* HackManager::pLocal()
{
	return pLocalInstance;
}

// Makes sure none of the hacks are called in their 
// hooks until they are completely ready for use
void HackManager::Ready()
{
	IsReady = true;
}

// https://steamcommunity.com/id/simplerealistic/