#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"
#include "RageBot.h"
#include "Crosshair.h"

#include "AnimationFix.h"

#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "Resolver.h"
#include "PlayerList.h"
#include "Globals.h"
#include "QAngle.hpp"
#include "intrin.h"
#include "SkinChanger.h"
#include "Datamanager.h"
#include "Utils.h"



bool thirdpersonBool = false;

Vector LastAngleAA;

float Bolbilize(float Yaw) //standard normalize func i made
{
	if (Yaw > 180)
	{
		Yaw -= (round(Yaw / 360) * 360.f);
	}
	else if (Yaw < -180)
	{
		Yaw += (round(Yaw / 360) * -360.f);
	}
	return Yaw;
}

Vector CalcAngle69(Vector dst, Vector src) //bolbi's local func
{
	Vector angles;

	double delta[3] = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	angles.x = (float)(atan(delta[2] / hyp) * 180.0 / 3.14159265);
	angles.y = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
	angles.z = 0.0f;

	if (delta[0] >= 0.0)
	{
		angles.y += 180.0f;
	}

	return angles;
}

// Funtion Typedefs
typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef bool(__thiscall *FireEventClientSideFn)(void*, IGameEvent*);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef void(__cdecl* MsgFn)(const char* msg, va_list);
typedef float(__stdcall *oGetViewModelFOV)();
typedef void(__thiscall* RunCommand_)(PVOID, IClientEntity*, CUserCmd*, IMoveHelper*);

PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
FrameStageNotifyFn oFrameStageNotify;
FireEventClientSideFn oFireEventClientSide;

float Bolbilize(float Yaw);

Vector CalcAngle69(Vector dst, Vector src);

void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __fastcall Hooked_FireEventClientSide(void *ecx, void* edx, IGameEvent* pEvent);
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
float __stdcall GGetViewModelFOV();
void RecvLowerBodyYawTarget(const CRecvProxyData *pData, IClientEntity* entity, void *pOut);
void KnifeProxy(CRecvProxyData *pData, void *pStruct, void *pOut);
IMaterial* __fastcall Hooked_FindMaterial(void* pThis, void*, const char* pMaterialName, const char* pTextureGroup, bool complain, const char* pComplainPrefix);
void __stdcall Hooked_RunCommand(IClientEntity* pPlayer, CUserCmd* pCmd, IMoveHelper* pMoveHelper);



namespace Hooks
{
	// VMT Managers
	U::Memory::VMTManager VMTPanel; // Hooking drawing functions
	U::Memory::VMTManager VMTClient; // Maybe CreateMove
	U::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	U::Memory::VMTManager VMTModelRender; // DrawModelEx for chams
	U::Memory::VMTManager VMTPrediction; // InPrediction for no vis recoil
	U::Memory::VMTManager VMTMaterialSystem;
};
void Hooks::Initialise()
{
	VMTPanel.Initialise((DWORD*)I::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);

	VMTPrediction.Initialise((DWORD*)I::Prediction);
	VMTPrediction.HookMethod((DWORD)Hooked_InPrediction, 14);
	VMTPrediction.HookMethod((DWORD)&Hooked_RunCommand, 19);

	VMTModelRender.Initialise((DWORD*)I::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.HookMethod((DWORD)Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);

	VMTClientMode.Initialise((DWORD*)I::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);

	oOverrideView = (OverrideViewFn)VMTClientMode.HookMethod((DWORD)&OverrideView, 18);
	VMTClientMode.HookMethod((DWORD)&GGetViewModelFOV, 35);

	VMTClient.Initialise((DWORD*)I::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 36);

	oFireEventClientSide = (FireEventClientSideFn)U::Memory::Hook(I::GameEventManager, 9, Hooked_FireEventClientSide);

	VMTMaterialSystem.Initialise((DWORD*)I::MaterialSystem);
	VMTMaterialSystem.HookMethod((DWORD)Hooked_FindMaterial, 84);

	
}
void Hooks::UndoHooks()
{
	VMTPanel.RestoreOriginal();
	VMTPrediction.RestoreOriginal();
	VMTModelRender.RestoreOriginal();
	VMTClientMode.RestoreOriginal();

	//VMTClient.RestoreOriginal();
}
void MovementCorrection(Vector& oang, CUserCmd* pCmd)
{
	Vector vMove = Vector(pCmd->forwardmove, pCmd->sidemove, 0.0f);
	float flSpeed = vMove.Length();
	Vector qMove;
	VectorAngles(vMove, qMove);
	float normalized = fmod(pCmd->viewangles.y + 180.f, 360.f) - 180.f;
	float normalizedx = fmod(pCmd->viewangles.x + 180.f, 360.f) - 180.f;
	float flYaw = DEG2RAD((normalized - oang.y) + qMove.y);
	if (normalizedx >= 90.0f || normalizedx <= -90.0f) pCmd->forwardmove = -cos(flYaw) * flSpeed;
	else pCmd->forwardmove = cos(flYaw) * flSpeed;
	pCmd->sidemove = sin(flYaw) * flSpeed;
}
void AngleClampingForBoolers(CUserCmd* pCmd, Vector OrigAngle)
{
	IClientEntity *pLocal = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());

	if (!miscconfig.bEnableUntrustedOptions)
	{
		GameUtils::NormaliseViewAngle(pCmd->viewangles);

		if (pCmd->viewangles.z != 0.0f)
		{
			pCmd->viewangles.z = 0.00;
		}
		if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				pCmd->viewangles = OrigAngle;
		}
	}
}

bool isscoped2 = false;
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{

	if (!strcmp("HudZoom", I::Panels->GetName(vguiPanel)) && Menu::Window.VisualsTab.NoScope.GetState() && hackManager.pLocal()->IsScoped() && I::Engine->IsConnected() && I::Engine->IsInGame() && hackManager.pLocal()->IsAlive())
	{
		return;
		isscoped2 = true;
	}

	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int OverlayerPan = 0;
	static bool FoundPanel = false;

	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)I::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, XorStr("MatSystemTopPanel")))
		{
			OverlayerPan = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (OverlayerPan == vguiPanel)
	{

		if (I::Engine->CIsTakingScreenshot() && Menu::Window.VisualsTab.Antiscreenshot.GetState())
			return;


		if (I::Engine->IsConnected() && I::Engine->IsInGame())
		{
			Hacks::DrawHacks();
			if (visualconfig.bHitmarkers)
				hitmarkers.Draw();
			crosshair.Draw();
		}

		nightmode.SetModulations();
		Menu::DoUIFrame();

		settings.Update();
		globals.forecolor = Color(0, 120, 255, 255);
	}
}
bool __stdcall Hooked_InPrediction()
{
	IClientEntity* pLocalEntity = NULL;
	float* m_LocalViewAngles = NULL;

	__asm
	{
		MOV pLocalEntity, ESI
		MOV m_LocalViewAngles, EBX
	}

	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::VMTPrediction.GetOriginalFunction(14);
	static DWORD *ecxVal = (DWORD*)I::Prediction;
	DWORD dwCalcPlayerView = Offsets::Functions::dwCalcPlayerView;
	result = origFunc(ecxVal);

	// If we are in the right place where the player view is calculated
	// Calculate the change in the view and get rid of it
	if ((DWORD)(_ReturnAddress()) == dwCalcPlayerView && visualconfig.bNoVisualRecoil)
	{
		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();

		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}

	return result;
}

//DRM for chams
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
}

bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number)
		return true;

	if (I::Engine->IsConnected() && I::Engine->IsInGame())
	{
		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;

		IClientEntity *pLocal = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());
		if (I::Engine->IsConnected() && I::Engine->IsInGame() && pLocal)
		{
			Vector origView = pCmd->viewangles;
			Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
			Vector qAimAngles;
			qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
			AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
			Vector ClientAngles; I::Engine->GetViewAngles(ClientAngles);

			for (int i = 1; i < I::Engine->GetMaxClients(); i++) {
				IClientEntity* pBaseEntity = I::EntList->GetBaseEntity(i);
				if (pBaseEntity && !pBaseEntity->IsDormant() && pBaseEntity != hackManager.pLocal()) globals.OldSimulationTime[i] = pBaseEntity->GetSimulationTime();
				if (pBaseEntity && !pBaseEntity->IsDormant() && pBaseEntity != hackManager.pLocal() && resolverconfig.bBacktrack) DataManager.UpdatePlayerPos();
			}

			if (pLocal && pLocal->GetActiveWeaponHandle())
				StartPrediction(pCmd);

			Hacks::MoveHacks(pCmd, bSendPacket);

			CBaseCombatWeapon* weapon = hackManager.pLocal()->GetWeapon();
			if (weapon)
			{
				if (!weapon->IsGrenade() && !weapon->IsKnife() && !weapon->IsC4() && weapon->GetAmmoInClip() > 0 && globals.Target)
				{
					if (GameUtils::CanShoot() && pCmd->buttons & IN_ATTACK)
						globals.Shots += 1;
				}
			}

			lineLBY = *pLocal->GetLowerBodyYaw();
			if (bSendPacket == true) {
				lineFakeAngle = pCmd->viewangles.y;
			}
			else {
				lineRealAngle = pCmd->viewangles.y;
			}

			I::Engine->GetViewAngles(origView);
			if (!miscconfig.bEnableUntrustedOptions) { Utils::Clamp(pCmd->viewangles); }
			MovementCorrection(ClientAngles, pCmd);

			if (visualconfig.bThirdpersonAA)
			{
				globals.qLastTickAngle.x = pCmd->viewangles.x;
				if (!bSendPacket)
					globals.qLastTickAngle.y = pCmd->viewangles.y;
			}

			if (Menu::Window.MiscTab.ThirdPersonType.GetIndex() == 0)
			{
				if (bSendPacket)
					LastAngleAA = pCmd->viewangles;
			}
			else if (Menu::Window.MiscTab.ThirdPersonType.GetIndex() == 1)
			{
				if (!bSendPacket)
					LastAngleAA = pCmd->viewangles;
			}
		}
	}
	return false;
}

float RandomFloat(float min, float max)
{
	typedef float(*RandomFloat_t)(float, float);
	return ((RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat")) (min, max);
}

void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{
	DWORD eyeangles = NetVar.GetNetVar(0xBFEA4E7B);

	IClientEntity *pLocal = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());

	if (I::Engine->IsConnected() && I::Engine->IsInGame() && curStage == FRAME_RENDER_START)
	{

		int thirdpersonKey = 0x56;
		if (thirdpersonKey > 0 && GUI.GetKeyPress(thirdpersonKey))
		{
			thirdpersonBool = !thirdpersonBool;
		}

		if ((Menu::Window.MiscTab.ThirdPerson.GetState()))
		{
			if (thirdpersonBool)
			{
				static bool rekt = false;
				if (!rekt)
				{
					ConVar* sv_cheats = I::CVar->FindVar("sv_cheats");
					SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
					sv_cheats_spoofed->SetInt(1);
					rekt = true;
				}
			}
			static bool rekt1 = false;
			if (thirdpersonBool && pLocal->IsAlive() && pLocal->IsScoped() == 0)
			{
				if (!rekt1)
				{
					I::Engine->ClientCmd_Unrestricted("thirdperson");
					rekt1 = true;
				}
			}
			else if (!thirdpersonBool)
			{
				rekt1 = false;
			}

			static bool rekt = false;
			if (!thirdpersonBool || pLocal->IsAlive() == 0)
			{
				if (!rekt)
				{
					I::Engine->ClientCmd_Unrestricted("firstperson");
					rekt = true;
				}
			}
			else if (thirdpersonBool || pLocal->IsAlive() || pLocal->IsScoped() == 0)
			{
				rekt = false;
			}

			static bool meme = false;
			if (thirdpersonBool && pLocal->IsScoped() == 0)
			{
				if (!meme)
				{
					I::Engine->ClientCmd_Unrestricted("thirdperson");
					meme = true;
				}
			}
			else if (pLocal->IsScoped())
			{
				meme = false;
			}

			static bool kek = false;
			if (thirdpersonBool && pLocal->IsAlive())
			{
				if (!kek)
				{
					I::Engine->ClientCmd_Unrestricted("thirdperson");
					kek = true;
				}
			}
			else if (pLocal->IsAlive() == 0)
			{
				kek = false;
			}
		}
	}

	if (curStage == FRAME_RENDER_START)
	{
		if (I::Engine->IsConnected() && I::Engine->IsInGame())
		{
			if (hackManager.pLocal() && hackManager.pLocal()->IsAlive())
			{
				IClientEntity* pLocal = hackManager.pLocal();

				if (Menu::Window.MiscTab.ThirdPerson.GetState())
				{
					auto thirdperson = *(bool*)(reinterpret_cast<uintptr_t>(I::Input) + 0xA5);
					if (thirdperson)
					{
						*(Vector*)((DWORD)hackManager.pLocal() + 0x31C8) = LastAngleAA;
					}
				}

				for (int i = 1; i <= I::EntList->GetHighestEntityIndex(); i++)
				{
					if (i == I::Engine->GetLocalPlayer()) continue;

					IClientEntity* pEntity = I::EntList->GetClientEntity(i);

					if (!pEntity || pEntity->IsDormant() || !pEntity->IsAlive())
						continue;

					if (pEntity->GetTeamNum() == pLocal->GetTeamNum() || !pLocal->IsAlive())
						continue;

					*(int*)((uintptr_t)pEntity + 0xA30) = I::Globals->framecount;
					*(int*)((uintptr_t)pEntity + 0xA28) = 0;
				}
			}
		}

		nightmode.SetSkybox();
	}

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (I::Engine->IsInGame() && I::Engine->IsConnected())
		{
			IClientEntity* pLocal = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());

			for (auto i = 0; i < I::EntList->GetHighestEntityIndex(); i++)
			{
				IClientEntity *pEnt = (IClientEntity*)I::EntList->GetClientEntity(i);

				if (pEnt->IsValidPlayer() && hackManager.pLocal()->IsAlive())
				{
					if (resolverconfig.bBacktrack) DataManager.ReportMove(pEnt->GetAbsOrigin(), pEnt->GetIndex());

					if (resolverconfig.iResolverMode == 1) // Beta
					{
						static float oldlby[65];
						bool lby_updated;
						const bool is_moving = pEnt->GetVelocity().Length2D() > 0.1;
						static float last_moving_lby[64];
						int storedmissedshots = 0;

						if (*pEnt->GetLowerBodyYaw() != oldlby[pEnt->GetIndex()])
						{
							lby_updated = true;
							oldlby[pEnt->GetIndex()] = *pEnt->GetLowerBodyYaw();
						}
						else
						{
							lby_updated = false;
						}

						if (lby_updated)
						{
							ResolverStage[pEnt->GetIndex()] = 2;
							pEnt->GetEyeAngles()->y = *pEnt->GetLowerBodyYaw();
						}
						else if (is_moving)
						{
							ResolverStage[pEnt->GetIndex()] = 1;
							last_moving_lby[pEnt->GetIndex()] = *pEnt->GetLowerBodyYaw();
							pEnt->GetEyeAngles()->y = *pEnt->GetLowerBodyYaw();
							storedmissedshots = globals.Shots;
						}
						else
						{
							if (storedmissedshots < globals.Shots)
							{
								switch (globals.Shots % 3)
								{
								case 0: pEnt->GetEyeAngles()->y = Bolbilize(CalcAngle69(*pEnt->GetOriginPtr(), *pLocal->GetOriginPtr()).y + 70); ResolverStage[pEnt->GetIndex()] = 4; break;
								case 1: pEnt->GetEyeAngles()->y = Bolbilize(CalcAngle69(*pEnt->GetOriginPtr(), *pLocal->GetOriginPtr()).y - 180); ResolverStage[pEnt->GetIndex()] = 5; break;
								case 2: pEnt->GetEyeAngles()->y = Bolbilize(CalcAngle69(*pEnt->GetOriginPtr(), *pLocal->GetOriginPtr()).y - 70); ResolverStage[pEnt->GetIndex()] = 6; break;
								}
							}
							else
							{
								ResolverStage[pEnt->GetIndex()] = 3;
								pEnt->GetEyeAngles()->y = last_moving_lby[pEnt->GetIndex()];
							}
						}
					}
					if (resolverconfig.iResolverMode == 2) // test before release
					{
						static float oldlby[65];
						static float lastlby[65];
						static float standtime[65];
						static float lbystandtime[65];
						static float last_moving_time[65];
						bool lby_updated;
						float bodyeyedelta = pEnt->GetEyeAngles()->y - *pEnt->GetLowerBodyYaw();
						const bool is_moving = pEnt->GetVelocity().Length2D() > 0.1;
						const bool could_be_slowmo = pEnt->GetVelocity().Length2D() < 40;
						const bool is_crouching = *pEnt->GetFlags() & FL_DUCKING;
						static float last_moving_lby[64];
						standtime[pEnt->GetIndex()] = I::Globals->curtime - last_moving_time[pEnt->GetIndex()];
						lbystandtime[pEnt->GetIndex()] = I::Globals->curtime - last_moving_time[pEnt->GetIndex()];

						if (lbystandtime[pEnt->GetIndex()] >= 1.1)
						{
							lbystandtime[pEnt->GetIndex()] -= 1.1;
						}
						if (*pEnt->GetLowerBodyYaw() != oldlby[pEnt->GetIndex()])
						{
							lby_updated = true;
							oldlby[pEnt->GetIndex()] = *pEnt->GetLowerBodyYaw();
						}
						else
						{
							lby_updated = false;
						}

						if (lby_updated)
						{
							pEnt->GetEyeAngles()->y = *pEnt->GetLowerBodyYaw();
						}
						else if (is_moving && !could_be_slowmo)
						{
							ResolverStage[pEnt->GetIndex()] = 1;
							last_moving_lby[pEnt->GetIndex()] = *pEnt->GetLowerBodyYaw();
							lastlby[pEnt->GetIndex()] = last_moving_lby[pEnt->GetIndex()];
							last_moving_time[pEnt->GetIndex()] = I::Globals->curtime;
							pEnt->GetEyeAngles()->y = *pEnt->GetLowerBodyYaw();
						}
						else if (lbystandtime[pEnt->GetIndex()] > 1.05 && lbystandtime[pEnt->GetIndex()] < 1.125)
						{
							ResolverStage[pEnt->GetIndex()] = 4;
							pEnt->GetEyeAngles()->y = *pEnt->GetLowerBodyYaw();
						}
						else if (is_moving && !is_crouching)
						{
							ResolverStage[pEnt->GetIndex()] = 2;
							pEnt->GetEyeAngles()->y = last_moving_lby[pEnt->GetIndex()];
						}
						else
						{
							if ((standtime[pEnt->GetIndex()]) > 7)
							{
								ResolverStage[pEnt->GetIndex()] = 3;
								switch (globals.Shots % 3)
								{
								case 0: pEnt->GetEyeAngles()->y = Bolbilize(CalcAngle69(*pEnt->GetOriginPtr(), *pLocal->GetOriginPtr()).y + 120); break;
								case 1: pEnt->GetEyeAngles()->y = Bolbilize(CalcAngle69(*pEnt->GetOriginPtr(), *pLocal->GetOriginPtr()).y + 180); break;
								case 2: pEnt->GetEyeAngles()->y = Bolbilize(CalcAngle69(*pEnt->GetOriginPtr(), *pLocal->GetOriginPtr()).y - 120); break;
								}
							}
							else
							{
								ResolverStage[pEnt->GetIndex()] = 2;
								switch (globals.Shots % 3)
								{
								case 0: pEnt->GetEyeAngles()->y = last_moving_lby[pEnt->GetIndex()]; break;
								case 1: pEnt->GetEyeAngles()->y = last_moving_lby[pEnt->GetIndex()] + 180; break;
								}
							}
						}
					}
				}
			}
		}
	}

	oFrameStageNotify(curStage);
}
OverrideViewFn oOverrideView;
void __stdcall Hooks::OverrideView(CViewSetup* vsView)
{
	if (I::Engine->IsConnected() && I::Engine->IsInGame())
	{
		IClientEntity* pLocal = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());

		if (pLocal->IsAlive() && !pLocal->IsScoped())
		{
			if (visualconfig.bWeaponScope && GetAsyncKeyState(VK_RBUTTON))
				vsView->fov = 65;
			else
			{
				if (visualconfig.flFoVAmt <= 1)
					vsView->fov = 90;
				else
					vsView->fov = visualconfig.flFoVAmt + 90;
			}
		}

		globals.fov = vsView->fov;
	}

	oOverrideView(vsView);
}
void __stdcall Hooked_RunCommand(IClientEntity* pPlayer, CUserCmd* pCmd, IMoveHelper* pMoveHelper)
{
	((RunCommand_)Hooks::VMTPrediction.GetOriginalFunction(19))(I::Prediction, pPlayer, pCmd, pMoveHelper);

	I::MoveHelper = pMoveHelper;
}
IMaterial* __fastcall Hooked_FindMaterial(void* pThis, void*, const char* pMaterialName, const char* pTextureGroup, bool complain, const char* pComplainPrefix)
{
	IMaterial* pMaterial = Hooks::VMTMaterialSystem.GetMethod<IMaterial*(__thiscall*)(void*, const char*, const char*, bool, const char*)>(84)(pThis, pMaterialName, pTextureGroup, complain, pComplainPrefix);
	if (pMaterial && pMaterialName && pTextureGroup)
	{
		if (strstr(pTextureGroup, "World"))
			nightmode.materials_world.push_back(pMaterial);
		if (strstr(pTextureGroup, "StaticProp"))
			nightmode.materials_props.push_back(pMaterial);
	}
	return pMaterial;
}
bool __fastcall Hooked_FireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent) {

	if (!pEvent)
		return oFireEventClientSide(ecx, pEvent);

	if (!strcmp(pEvent->GetName(), "player_death"))
	{
		int attacker = I::Engine->GetPlayerForUserID(pEvent->GetInt("attacker"));
		int userid = I::Engine->GetPlayerForUserID(pEvent->GetInt("userid"));

		if (miscconfig.bKillMessage)
		{
			if (attacker != userid)
			{
				if (attacker == I::Engine->GetLocalPlayer())
				{
					globals.killspammer.killedPlayerQueue.push_back(userid);
				}
			}
		}
	}

	if (!strcmp(pEvent->GetName(), "player_hurt"))
	{
		if (I::Engine->GetPlayerForUserID(pEvent->GetInt("attacker", 0)) == I::Engine->GetLocalPlayer())
			hitmarkers.bDoHitmarker = true;
	}

	if (!strcmp(pEvent->GetName(), "round_start"))
		hitmarkers.bDoRoundStart = true;

	return oFireEventClientSide(ecx, pEvent);
}
void GetViewModelFOV(float& fov)
{
	IClientEntity* localplayer = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());

	if (I::Engine->IsConnected() && I::Engine->IsInGame())
	{
		if (!localplayer)
			return;

		if (visualconfig.bViewmodelFoV)
			fov += visualconfig.flViewmodelFoVAmt;
	}
}
float __stdcall GGetViewModelFOV()
{
	float fov = Hooks::VMTClientMode.GetMethod<oGetViewModelFOV>(35)();
	if (I::Engine->IsConnected() && I::Engine->IsInGame())
		GetViewModelFOV(fov);

	return fov;
}







void Warning(const char* msg, ...)
{
	if (msg == nullptr)
		return;
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle(XorStr("tier0.dll")), XorStr("Warning"));
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list);
}