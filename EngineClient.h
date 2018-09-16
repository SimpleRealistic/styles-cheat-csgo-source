
#pragma once

#include "MiscDefinitions.h"
#include "ClientRecvProps.h"
#include "offsets.h"
#include "Vector.h"

enum FlowType_t
{
	FLOW_OUTGOING = 0,
	FLOW_INCOMING = 1,
	FLOW_MAX = 2
};

class INetChannelInfo
{
public:
	float GetLatency(int flow)
	{
		typedef float(__thiscall* GetLatencyFn)(void*, int);
		return GetMethod<GetLatencyFn>(this, 9)(this, flow);
	}

	float GetAvgLatency(int flow)
	{
		typedef float(__thiscall* GetAvgLatencyFn)(void*, int);
		return GetMethod<GetAvgLatencyFn>(this, 10)(this, flow);
	}
};

// EngineClient
class IVEngineClient
{
public:
	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* oGetScreenSize)(PVOID, int&, int&);
		return call_vfunc< oGetScreenSize >(this, Offsets::VMT::Engine_GetScreenSize)(this, width, height);
	}

	bool GetPlayerInfo(int ent_num, player_info_t *pinfo)
	{
		typedef bool(__thiscall* oGetPlayerInfo)(PVOID, int, player_info_t*);
		return call_vfunc< oGetPlayerInfo >(this, Offsets::VMT::Engine_GetPlayerInfo)(this, ent_num, pinfo);
	}

	bool GetPlayerInfo(int ent_num, player_info *pinfo)
	{
		typedef bool(__thiscall* oGetPlayerInfo)(PVOID, int, player_info*);
		return call_vfunc< oGetPlayerInfo >(this, Offsets::VMT::Engine_GetPlayerInfo)(this, ent_num, pinfo);
	}

	inline int GetPlayerForUserID(int UserID) {
		return call_vfunc<bool(__thiscall *)(void *, int)>(this, 9)(this, UserID);
	}

	int GetLocalPlayer()
	{
		typedef int(__thiscall* oLocal)(PVOID);
		return call_vfunc< oLocal >(this, Offsets::VMT::Engine_GetLocalPlayer)(this);
	}
	float Time()
	{
		typedef float(__thiscall* oTime)(PVOID);
		return call_vfunc< oTime >(this, Offsets::VMT::Engine_Time)(this);
	}
	void GetViewAngles(Vector& vAngles)
	{
		typedef void(__thiscall* oSetViewAngles)(PVOID, Vector&);
		return call_vfunc< oSetViewAngles >(this, Offsets::VMT::Engine_GetViewAngles)(this, vAngles);
	}
	void SetViewAngles(Vector& vAngles)
	{
		typedef void(__thiscall* oSetViewAngles)(PVOID, Vector&);
		return call_vfunc< oSetViewAngles >(this, Offsets::VMT::Engine_SetViewAngles)(this, vAngles);
	}
	int GetMaxClients()
	{
		typedef bool(__thiscall* oGetMaxClients)(PVOID);
		return call_vfunc< oGetMaxClients >(this, Offsets::VMT::Engine_GetMaxClients)(this);
	}
	bool IsConnected()
	{
		typedef bool(__thiscall* oGetScreenSize)(PVOID);
		return call_vfunc< oGetScreenSize >(this, Offsets::VMT::Engine_IsConnected)(this);
	}
	bool IsInGame()
	{
		typedef bool(__thiscall* oLocal)(PVOID);
		return call_vfunc< oLocal >(this, Offsets::VMT::Engine_IsInGame)(this);
	}
	const matrix3x4& WorldToScreenMatrix()
	{
		typedef const matrix3x4& (__thiscall* oWorldToScreenMatrix)(PVOID);
		return call_vfunc< oWorldToScreenMatrix >(this, Offsets::VMT::Engine_WorldToScreenMatrix)(this);
	}
	void ClientCmd_Unrestricted(char  const* cmd)
	{
		typedef void(__thiscall* oClientCmdUnres)(PVOID, const char*);
		return call_vfunc<oClientCmdUnres>(this, Offsets::VMT::Engine_ClientCmd_Unrestricted)(this, cmd);
	}
	INetChannelInfo* GetNetChannelInfo(void)
	{
		typedef INetChannelInfo*(__thiscall* OriginalFn)(PVOID);
		return call_vfunc< OriginalFn >(this, 78)(this);
	}
	bool CIsTakingScreenshot() {
		typedef bool(__thiscall* oScrShot)(PVOID);
		return call_vfunc<oScrShot>(this, 92)(this);
	}
	float GetLastTimeStamp() {
		typedef float(__thiscall *OrigFn)(void*);
		return call_vfunc<OrigFn>(this, 14)(this);
	}
};