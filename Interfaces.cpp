#include "Interfaces.h"
#include "Utilities.h"

CreateInterfaceFn fnEngineFactory = NULL;

template< class T > T* GetInterface(std::string szModuleName, std::string szInterfaceName, bool bSkip = false)
{
	if (szModuleName.empty() || szInterfaceName.empty())
		return nullptr;
	typedef PVOID(*CreateInterfaceFn)(const char* pszName, int* piReturnCode);
	CreateInterfaceFn hInterface = nullptr;
	while (!hInterface)
	{
		hInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(szModuleName.c_str()), "CreateInterface");
		Sleep(5);
	}

	char pszBuffer[256];
	for (int i = 0; i < 100; i++)
	{
		sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i);
		PVOID pInterface = hInterface(pszBuffer, nullptr);

		if (pInterface && pInterface != NULL)
		{
			if (bSkip)
				sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i + 1);

			Sleep(5);
			break;
		}
	}

	return (T*)hInterface(pszBuffer, nullptr);
}

void I::Initialise()
{
	fnEngineFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(XorStr("engine.dll")), XorStr("CreateInterface"));

	GameEventManager = (IGameEventManager2*)fnEngineFactory(XorStr("GAMEEVENTSMANAGER002"), NULL);

	if (!Client) { Client = GetInterface<IBaseClientDLL>(XorStr("client.dll"), XorStr("VClient")); }

	if (!Engine) { Engine = GetInterface<IVEngineClient>(XorStr("engine.dll"), XorStr("VEngineClient")); }

	if (!Panels) { Panels = GetInterface<IPanel>(XorStr("vgui2.dll"), XorStr("VGUI_Panel")); }

	if (!EntList) { EntList = GetInterface<IClientEntityList>(XorStr("client.dll"), XorStr("VClientEntityList")); }

	if (!Surface) { Surface = GetInterface<ISurface>(XorStr("vguimatsurface.dll"), XorStr("VGUI_Surface")); }

	if (!DebugOverlay) { DebugOverlay = GetInterface<IVDebugOverlay>(XorStr("engine.dll"), XorStr("VDebugOverlay")); }

	if (!Prediction) { Prediction = GetInterface<IPrediction>(XorStr("client.dll"), XorStr("VClientPrediction")); }

	if (!MaterialSystem) { MaterialSystem = GetInterface<CMaterialSystem>(XorStr("materialsystem.dll"), XorStr("VMaterialSystem")); }

	if (!RenderView) { RenderView = GetInterface<CVRenderView>(XorStr("engine.dll"), XorStr("VEngineRenderView")); }

	if (!ModelRender) { ModelRender = GetInterface<IVModelRender>(XorStr("engine.dll"), XorStr("VEngineModel")); }

	if (!ModelInfo) { ModelInfo = GetInterface<CModelInfo>(XorStr("engine.dll"), XorStr("VModelInfoClient")); }

	if (!Trace) { Trace = GetInterface<IEngineTrace>(XorStr("engine.dll"), XorStr("EngineTraceClient")); }

	if (!PhysProps) { PhysProps = GetInterface<IPhysicsSurfaceProps>(XorStr("vphysics.dll"), XorStr("VPhysicsSurfaceProps")); }

	if (!CVar) { CVar = GetInterface<ICVar>(XorStr("vstdlib.dll"), XorStr("VEngineCvar")); }

	if (!GameMovement) { GameMovement = GetInterface<IGameMovement>(XorStr("client.dll"), XorStr("GameMovement")); }

	if (!PredictionRandomSeed) { PredictionRandomSeed = *(int**)(U::Memory::FindPatternVersion2("client.dll", "89 35 ? ? ? ? F3 0F 10 48 20") + 2); }

	ClientMode = **(IClientMode***)((*(DWORD**)Client)[10] + 0x5);
	Globals = **reinterpret_cast< CGlobalVarsBase*** >((*reinterpret_cast< DWORD** >(Client))[0] + 0x1B);

	auto uAddress = U::Memory::FindPatternVersion2(XorStr("client.dll"), XorStr("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10")) + 1;
	Input = *(CInput**)(uAddress);

}

namespace I
{
	IBaseClientDLL* Client = nullptr;
	IVEngineClient* Engine = nullptr;
	IPanel* Panels = nullptr;
	IClientEntityList* EntList = nullptr;
	ISurface* Surface = nullptr;
	IVDebugOverlay* DebugOverlay = nullptr;
	IClientMode *ClientMode = nullptr;
	CGlobalVarsBase *Globals = nullptr;
	IPrediction *Prediction = nullptr;
	CMaterialSystem* MaterialSystem = nullptr;
	CVRenderView* RenderView = nullptr;
	IVModelRender* ModelRender = nullptr;
	CModelInfo* ModelInfo = nullptr;
	IEngineTrace* Trace = nullptr;
	IPhysicsSurfaceProps* PhysProps = nullptr;
	ICVar *CVar = nullptr;
	NetvarManager *NetVarMg = nullptr;
	CInput* Input = nullptr;
	IGameEventManager2*	GameEventManager = nullptr;
	IMoveHelper* MoveHelper = nullptr;
	IGameMovement* GameMovement = nullptr;
	IPrediction* GamePrediction = nullptr;
	CInput* pInput = nullptr;

	int* PredictionRandomSeed; // 
};