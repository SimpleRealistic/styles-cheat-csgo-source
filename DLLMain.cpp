

// General shit
#include "DLLMain.h"
#include "Utilities.h"
#include "Nosmoke.h"

// Injection stuff
#include "INJ/ReflectiveLoader.h"

// Stuff to initialise
#include "Interfaces.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"
#include "Menu.h"
#include "MiscHacks.h"
#include "Dumping.h"
#include "mac.h"
#include "Vaults.h"
#include "Anti Leak.h"
#include <Windows.h>

UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\"; // location HWID

extern HINSTANCE hAppInstance;

int LBYBreakerTimer;
float lineLBY;
float lineRealAngle;
float lineFakeAngle;
int ResolverStage[65];

// Our DLL Instance
HINSTANCE HThisModule;
bool DoUnload;

void CAntiLeak::ErasePE()
{
	char *pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0;
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}


int InitialThread()
{
	Offsets::Initialise();
	I::Initialise();
	NetVar.RetrieveClasses();
	NetvarManager::Instance()->CreateDatabase();
	Draw::Initialise();
	Hacks::SetupHacks();
	Menu::SetupMenu();
	Hooks::Initialise();
	ApplyAAAHooks();
	ApplyNetVarsHooks();

	while (DoUnload == false)
		Sleep(1000);

	Hooks::UndoHooks();
	Sleep(2000);
	FreeLibraryAndExitThread(HThisModule, 0);

	return 0;
}
typedef void(__cdecl* MsgFn)(const char* msg, va_list);
void Msg(const char* msg, ...)
{
	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); //This gets the address of export "Msg" in the dll "tier0.dll". The static keyword means it's only called once and then isn't called again (but the variable is still there)
	char buffer[989];
	va_list list; //Normal varargs stuff http://stackoverflow.com/questions/10482960/varargs-to-printf-all-arguments
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.
}
player_info_t GetInfo(int Index) {
	player_info_t Info;
	I::Engine->GetPlayerInfo(Index, &Info);
	return Info;
}

void imfinnarunuponya(IGameEvent* pEvent)
{
	int attackerid = pEvent->GetInt("attacker");
	int entityid = I::Engine->GetPlayerForUserID(attackerid);
	/*if (Menu::Window.MiscTab.KillMessage.GetState())
	{

		if (entityid == I::Engine->GetLocalPlayer())
		{
			hitmarkertime = 255;

			PlaySoundA(skeet, NULL, SND_ASYNC | SND_MEMORY);


		}
	}*/


	if (entityid == I::Engine->GetLocalPlayer())
	{
		int nUserID = pEvent->GetInt("attacker");
		int nDead = pEvent->GetInt("userid");
		if (nUserID || nDead)
		{
			player_info_t killed_info = GetInfo(I::Engine->GetPlayerForUserID(nDead));
			player_info_t killer_info = GetInfo(I::Engine->GetPlayerForUserID(nUserID));
			std::string before = ("[xInterface] ");
			std::string two = ("Hit ");
			std::string three = killed_info.name;
			std::string foura = " for ";
			std::string fivea = pEvent->GetString("dmg_health");
			std::string damage = " damage";
			std::string fourb = " (";
			std::string fiveb = pEvent->GetString("health");
			std::string six = " health remaining)";
			std::string newline = "\n";
			if (Menu::Window.MiscTab.Logs.GetState())
			{
				Msg((before + two + three + foura + fivea + damage + fourb + fiveb + six + newline).c_str());
			}
		}
	}
}



void imfinnakillyou(IGameEvent* pEvent)
{
	int nUserID = pEvent->GetInt("attacker");
	int nDead = pEvent->GetInt("userid");
	if (nUserID || nDead)
	{
		player_info_t killed_info = GetInfo(I::Engine->GetPlayerForUserID(nDead));
		player_info_t killer_info = GetInfo(I::Engine->GetPlayerForUserID(nUserID));
		std::string before = ("[xInterface] ");
		std::string one = killed_info.name;
		std::string two = (" bought ");
		std::string three = pEvent->GetString("weapon");
		std::string six = "\n";
		if (Menu::Window.MiscTab.Logs.GetState())
		{
			Msg((before + one + two + three + six).c_str());
		}
	}
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:

		if (GetModuleHandleA(XorStr("csgo.exe")))
		{
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);
		}
		else
		{
			ExitProcess(NULL);
			return TRUE;
		}
	}
	return TRUE;
}