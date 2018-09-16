

#pragma once

#include "Hacks.h"
#include "Settings.h"
#include "Vector4D.h"
#include "Globals.h"

class CEsp : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool& bSendPacket);
private:
	struct EBox
	{
		int x, y, w, h;
	};

	struct ESPBox
	{
		int x, y, w, h;
	};

	EBox CEsp::DynamicBox(IClientEntity* pEntity, bool& PVS);
	Color GetPlayerColor(IClientEntity* pEntity);

	void ManagePlayer(IClientEntity* entity, player_info_t pInfo);
	void Draw2DBox(EBox box, IClientEntity* pEntity, Color color);
	void DrawBox(CEsp::EBox size, Color color);
	void DrawHealth1(IClientEntity * pEntity, CEsp::EBox size);
	//void DrawHealth(IClientEntity * pEntity, CEsp::ESPBox size);
	//void DrawHealth(EBox box, IClientEntity* pEntity);
	//void DrawInfo(IClientEntity * pEntity, CEsp::EBox size);
	void DrawInfo(IClientEntity * pEntity, CEsp::EBox size, player_info_t pinfo);
	void DrawHealth(IClientEntity * pEntity, CEsp::EBox size);
	void DrawArmor(EBox box, IClientEntity* pEntity);
	void BulletTrace(IClientEntity * pEntity, Color color);
	void DrawDistance(CEsp::EBox size, IClientEntity * pEntity);
	float DistanceTo(Vector vecSrc, Vector vecDst);
	void DrawBoneTest(IClientEntity * pEntity);
	void DrawSkeleton(IClientEntity* pEntity);
	void DrawName(EBox box, IClientEntity* pEntity, player_info_t pinfo);
	void DrawLinesAA(Color color);

	void DrawGlow();
};

