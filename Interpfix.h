#pragma once

namespace InterpFix {
	typedef bool(__thiscall* IsPlayingTimeDemo_t)(void*);

	extern IsPlayingTimeDemo_t g_fnOriginalPlayingTimeDemo;

	bool __fastcall HookedIsPlayingTimeDemo(void* thisptr, void* edx);
}
