#include "RageBot.h"
#include "Interpfix.h"

namespace InterpFix {
	IsPlayingTimeDemo_t g_fnOriginalPlayingTimeDemo;
	bool __fastcall HookedIsPlayingTimeDemo(void* thisptr, void* edx) {
		bool bResult = g_fnOriginalPlayingTimeDemo(thisptr);

		if (resolverconfig.bInterpLagComp) {

			IClientEntity *pLocalPlayer = hackManager.pLocal();

			if (I::Engine->IsInGame() && I::Engine->IsConnected())
				if (pLocalPlayer->IsAlive() && !pLocalPlayer->InBuyZone())
					return true;
		}
		return bResult;
	}
}