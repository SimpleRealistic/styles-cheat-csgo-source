#pragma once
#include "Entities.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hacks.h"
#include "XorStr.hpp"

#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / I::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t ) ( I::Globals->interval_per_tick *( t ) )

void StartLagCompensation(CUserCmd* pCmd)
{
	if (Menu::Window.RageBotTab.BreakEU.GetState())
	{
		for (int i = 0; i < I::EntList->GetMaxEntities(); i++)
		{
			IClientEntity* pLocal = hackManager.pLocal();
			IClientEntity* pEntity = I::EntList->GetClientEntity(i);
			float flSimTime = pLocal->GetSimulationTime();

			pCmd->tick_count = TIME_TO_TICKS(flSimTime + 0.031f); // +1 Send
		}
	}
}