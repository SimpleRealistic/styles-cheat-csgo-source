
#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "GlowManager.h"
#include "Hooks.h"

#ifndef minimum
#define minimum(a,b)            (((a) < (b)) ? (a) : (b))
#endif
DWORD GlowManager = *(DWORD*)(U::Memory::FindPatternVersion2("client.dll", "A1 ? ? ? ? A8 01 75 4B") + 0x19);

//DWORD g_pGlowObjectManager = *(DWORD*)(U::Memory::FindPatternVersion2(XorStr("client.dll"), XorStr("0F 57 C0 C7 05 ? ? ? ? 00 00 00 00 0F 11 05 ? ? ? ? 68 ? ? ? ? 0F 28 05 ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? 00 00 00 00 C7 05 ? ? ? ? 00 00 00 00")) + 16);




void CEsp::Init()
{
}

// Yeah dude we're defo gunna do some sick moves for the esp yeah
void CEsp::Move(CUserCmd *pCmd, bool& bSendPacket)
{
}

void CEsp::Draw()
{
	if (hackManager.pLocal() && hackManager.pLocal()->IsAlive())
	{
		Color color;

		if (Menu::Window.VisualsTab.linesaa.GetState())
			DrawLinesAA(color);
	}

	for (int i = 1; i < I::Engine->GetMaxClients(); i++)
	{
		IClientEntity* pEntity = I::EntList->GetClientEntity(i);
		player_info_t pinfo;

		if (pEntity->IsValidESP())
		{
			if (I::Engine->GetPlayerInfo(i, &pinfo))
			{
				if (!visualconfig.bEnemyEspOnly && pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || visualconfig.bEnemyEspOnly)
					ManagePlayer(pEntity, pinfo);
			}
		}
	}
}

std::string CleanItemName(std::string name)
{
	int i = 1;

	if (i == 1)
	{
		i = 2;
	}

	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}
void CEsp::ManagePlayer(IClientEntity* pEntity, player_info_t pInfo)
{
	Color color;
	EBox box;

	int iScreenWidth, iScreenHeight;
	I::Engine->GetScreenSize(iScreenWidth, iScreenHeight);

	if (I::Engine->IsInGame() && I::Engine->IsConnected())
	{
		color = GetPlayerColor(pEntity);
		bool PVS = false;
		box = DynamicBox(pEntity, PVS);

		if (visualconfig.iEspMode == 1)
			Draw2DBox(box, pEntity, color);

		if (visualconfig.iEspMode == 2)
			DrawBox(box, color);

		if (visualconfig.bHealth)
			DrawHealth(pEntity, box);

		/*if (Menu::Window.VisualsTab.HealthBar.GetIndex() == 4 )
			DrawHealth1(pEntity, box);*/

		if (visualconfig.bArmor)
			DrawArmor(box, pEntity);

			DrawInfo(pEntity, box, pInfo);

		if (visualconfig.bSkeletons)
			DrawSkeleton(pEntity);

		if (Menu::Window.VisualsTab.Glow.GetState())
			DrawGlow();

		if (Menu::Window.VisualsTab.Distance.GetState())
			DrawDistance(box, pEntity);

		if (Menu::Window.VisualsTab.BulletTrace.GetState())
			BulletTrace(pEntity, color);
	}
}

CEsp::EBox CEsp::DynamicBox(IClientEntity* pPlayer, bool& PVS)
{
	EBox result;
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;
	vOrigin = pPlayer->GetAbsOrigin();
	min = pPlayer->collisionProperty()->GetMins() + vOrigin;
	max = pPlayer->collisionProperty()->GetMaxs() + vOrigin;
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };
	if (!Draw::WorldToScreen(points[3], flb) || !Draw::WorldToScreen(points[5], brt)
		|| !Draw::WorldToScreen(points[0], blb) || !Draw::WorldToScreen(points[4], frt)
		|| !Draw::WorldToScreen(points[2], frb) || !Draw::WorldToScreen(points[1], brb)
		|| !Draw::WorldToScreen(points[6], blt) || !Draw::WorldToScreen(points[7], flt))
		return result;
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;
	return result;
}

Color CEsp::GetPlayerColor(IClientEntity* pEntity)
{
	float red = Menu::Window.ColorsTab.CTBoxR.GetValue();
	float green = Menu::Window.ColorsTab.CTBoxG.GetValue();
	float blue = Menu::Window.ColorsTab.CTBoxB.GetValue();

	float red1 = Menu::Window.ColorsTab.TBoxR.GetValue();
	float green1 = Menu::Window.ColorsTab.TBoxG.GetValue();
	float blue1 = Menu::Window.ColorsTab.TBoxB.GetValue();

	float redTNV = Menu::Window.ColorsTab.TBoxRnv.GetValue();
	float greenTNV = Menu::Window.ColorsTab.TBoxGnv.GetValue();
	float blueTNV = Menu::Window.ColorsTab.TBoxBnv.GetValue();

	float redCTNV = Menu::Window.ColorsTab.CTBoxRnv.GetValue();
	float greenCTNV = Menu::Window.ColorsTab.CTBoxGnv.GetValue();
	float blueCTNV = Menu::Window.ColorsTab.CTBoxBnv.GetValue();

	int TeamNum = pEntity->GetTeamNum();
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head);

	Color color;

	if (TeamNum == TEAM_CS_CT)
		color = IsVis ? Color(red, green, blue, 255) : Color(redCTNV, greenCTNV, blueCTNV, 255);
	else
		color = IsVis ? Color(red1, green1, blue1, 255) : Color(redTNV, greenTNV, blueTNV, 255);

	return color;
}
void CEsp::Draw2DBox(EBox box, IClientEntity* pEntity, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawOutlinedRect(box.x, box.y, box.x + box.w, box.y + box.h);
	I::Surface->DrawSetColor(0, 0, 0, 185);
	I::Surface->DrawOutlinedRect(box.x - 1, box.y - 1, box.x + box.w + 1, box.y + box.h + 1);
	I::Surface->DrawOutlinedRect(box.x + 1, box.y + 1, box.x + box.w - 1, box.y + box.h - 1);
}

void CEsp::DrawBox(CEsp::EBox size, Color color)
{
	//if (PlayerBoxes->GetStringIndex() == 1)
	//{
	//	// Full Box
	//	renderMan->Outline(size.x, size.y, size.w, size.h, color);
	//	renderMan->Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, COLORCODE(10, 10, 10, 150));
	//	renderMan->Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, COLORCODE(10, 10, 10, 150));
	//}
	//else
	//{
	// Corner Box
	int VertLine = (((float)size.w) * (0.20f));
	int HorzLine = (((float)size.h) * (0.20f));

	Draw::Clear(size.x, size.y - 1, VertLine, 1, Color(10, 10, 10, 150));
	Draw::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(10, 10, 10, 150));
	Draw::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 150));
	Draw::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(10, 10, 10, 150));
	
	Draw::Clear(size.x - 1, size.y, 1, HorzLine, Color(10, 10, 10, 150));
	Draw::Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));
	Draw::Clear(size.x + size.w - 1, size.y, 1, HorzLine, Color(10, 10, 10, 150));
	Draw::Clear(size.x + size.w - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(10, 10, 10, 150));
	
	Draw::Clear(size.x, size.y, VertLine, 1, color);
	Draw::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Draw::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Draw::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);
	
	Draw::Clear(size.x, size.y, 1, HorzLine, color);
	Draw::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
	Draw::Clear(size.x + size.w, size.y, 1, HorzLine, color);
	Draw::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
	//}
}

void CEsp::DrawHealth1(IClientEntity* pEntity, CEsp::EBox size)
{
	EBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 6);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;

	// --  Main Bar -- //

	float flBoxes = std::ceil(pEntity->GetHealth() / 4); //change this number to change the amount of lines
	float flX = size.x - 7; float flY = size.y - 1;
	float flHeight = size.h / 10.f;
	float flMultiplier = 12 / 360.f; flMultiplier *= flBoxes - 1;
	Color ColHealth = Color::FromHSB(flMultiplier, 1, 1);
	int rectHeight = flHeight * flBoxes + 1;

	Draw::DrawRect(flX, flY, 4, size.h + 2, Color(80, 80, 80, 255));
	Draw::Outline(flX, flY, 4, size.h + 2, Color(0, 0, 0, 255));

	Draw::DrawRect(flX + 1, flY + size.h + (flMultiplier - rectHeight) + 1, 2, rectHeight, pEntity->IsDormant() ? Color(0, 0, 0, 140) : ColHealth);

	for (int i = 0; i < 4; i++) // change the 4 here to change the amount of lines has well
		Draw::Line(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, Color(0, 0, 0, 255));
}

void CEsp::DrawLinesAA(Color color) {
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = hackManager.pLocal();

	// LBY
	AngleVectors(Vector(0, lineLBY, 0), &forward);
	src3D = hackManager.pLocal()->GetAbsOrigin();
	dst3D = src3D + (forward * 30.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	I::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Draw::WorldToScreen(src3D, src) || !Draw::WorldToScreen(tr.endpos, dst))
		return;

	Draw::Line(src.x, src.y, dst.x, dst.y, Color(210, 105, 30, 255));
	// REAL AGNEL
	AngleVectors(Vector(0, lineRealAngle, 0), &forward);
	dst3D = src3D + (forward * 30.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	I::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Draw::WorldToScreen(src3D, src) || !Draw::WorldToScreen(tr.endpos, dst))
		return;

	Draw::Line(src.x, src.y, dst.x, dst.y, Color(0, 255, 0, 255));
	

	// Fake AGNEL
	AngleVectors(Vector(0, lineFakeAngle, 0), &forward);
	dst3D = src3D + (forward * 30.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	I::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Draw::WorldToScreen(src3D, src) || !Draw::WorldToScreen(tr.endpos, dst))
		return;

	Draw::Line(src.x, src.y, dst.x, dst.y, Color(255, 0, 0, 255));
}

void DrawLine(int x0, int y0, int x1, int y1, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawLine(x0, y0, x1, y1);
}

void DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	I::Surface->DrawSetColor(col);
	I::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

float CEsp::DistanceTo(Vector vecSrc, Vector vecDst)
{
	Vector vDelta = vecDst - vecSrc;

	float fDistance = ::sqrtf((vDelta.Length()));

	if (fDistance < 1.0f)
		return 1.0f;

	return fDistance;
}

void CEsp::DrawInfo(IClientEntity* pEntity, CEsp::EBox size, player_info_t pinfo)
{
	std::vector<std::string> Info;

	IClientEntity* pLocal = hackManager.pLocal();

	Vector vecOrigin = pEntity->GetOrigin();
	Vector vecOriginLocal = pLocal->GetOrigin();

	int HPEnemy = pEntity->GetHealth();

	float dist = DistanceTo(vecOriginLocal, vecOrigin) / 3;

	// Player Weapon ESP
	IClientEntity* pWeapon = I::EntList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (Menu::Window.VisualsTab.OptionsWeapon.GetState() && pWeapon)
	{
		ClientClass* cClass = (ClientClass*)pWeapon->GetClientClass();
		if (cClass)
		{
			// Draw it
			Info.push_back(CleanItemName(cClass->m_pNetworkName));
		}
	}

	int i = 0;

	static RECT Size = Draw::GetTextSize(Draw::Fonts::Default, "Hi");
	 
	char HPBuffer[512];
	sprintf_s(HPBuffer, "%d HP", HPEnemy);
	Draw::Text(size.x + size.w + 3, size.y + ( 1 * Size.bottom )- dist, Color(255, 255, 255, 255), Draw::Fonts::ESP, HPBuffer);

	if (visualconfig.bPlayerNames)
	Draw::Text(size.x + size.w + 3, size.y + ( 0.1 *Size.bottom) - dist, Color(255, 255, 255, 255), Draw::Fonts::ESP,  pinfo.name);
	
	if (ResolverStage[pEntity->GetIndex()] == 1) {
		static RECT Size = Draw::GetTextSize(Draw::Fonts::ESP, "Hi");
		Draw::Text(size.x + size.w + 3, size.y + (3.1 * Size.bottom) - dist, Color(255, 255, 255, 255), Draw::Fonts::ESP, "resolvemode 1");
	}
	else if (ResolverStage[pEntity->GetIndex()] == 2) {
		static RECT Size = Draw::GetTextSize(Draw::Fonts::ESP, "Hi");
		Draw::Text(size.x + size.w + 3, size.y + (3.1 * Size.bottom) - dist, Color(255, 255, 255, 255), Draw::Fonts::ESP, "resolvemode 2");
	}
	else if (ResolverStage[pEntity->GetIndex()] == 3) {
		static RECT Size = Draw::GetTextSize(Draw::Fonts::ESP, "Hi");
		Draw::Text(size.x + size.w + 3, size.y + (3.1 * Size.bottom) - dist, Color(255, 255, 255, 255), Draw::Fonts::ESP, "resolvemode 3");
	}
	else if (ResolverStage[pEntity->GetIndex()] == 4) {
		static RECT Size = Draw::GetTextSize(Draw::Fonts::ESP, "Hi");
		Draw::Text(size.x + size.w + 3, size.y + (3.1 * Size.bottom) - dist, Color(255, 255, 255, 255), Draw::Fonts::ESP, "resolvemode 4");
	}
	else if (ResolverStage[pEntity->GetIndex()] == 5) {
		static RECT Size = Draw::GetTextSize(Draw::Fonts::ESP, "Hi");
		Draw::Text(size.x + size.w + 3, size.y + (3.1 * Size.bottom) - dist, Color(255, 255, 255, 255), Draw::Fonts::ESP, "resolvemode 5");
	}
	else if (ResolverStage[pEntity->GetIndex()] == 6) {
		static RECT Size = Draw::GetTextSize(Draw::Fonts::ESP, "Hi");
		Draw::Text(size.x + size.w + 3, size.y + (3.1 * Size.bottom) - dist, Color(255, 255, 255, 255), Draw::Fonts::ESP, "resolvemode 6");
	}
	else {
		static RECT Size = Draw::GetTextSize(Draw::Fonts::ESP, "Hi");
		Draw::Text(size.x + size.w + 3, size.y + (3.1 * Size.bottom) - dist, Color(255, 255, 255, 255), Draw::Fonts::ESP, "no resolve");
	}

	for (auto Text : Info)
	{
		Draw::Text(size.x + size.w + 3, size.y + ( 2 * Size.bottom) - dist, Color(255, 255, 255, 255), Draw::Fonts::ESP, Text.c_str());
		i++;
	}
}

void CEsp::DrawHealth(IClientEntity* pEntity, CEsp::EBox size)
{
	EBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 6);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;
#define HealthType Menu::Window.VisualsTab.HealthBar.GetState()
	if (HealthType)
	{
		EBox HealthBar = size;
		HealthBar.y += (HealthBar.h + 6);
		HealthBar.h = 4;

		float HealthValue = pEntity->GetHealth();
		float HealthPerc = HealthValue / 100.f;
		float Width = (size.w * HealthPerc);
		HealthBar.w = Width;

		// --  Main Bar -- //

		float flBoxes = std::ceil(pEntity->GetHealth() / 10);
		float flX = size.x - 7; float flY = size.y - 1;
		float flHeight = size.h / 10.f;
		float flHeight2 = size.h / 10.f;
		float flMultiplier = 12 / 360.f; flMultiplier *= flBoxes - 1;
		Color ColHealth = Color::FromHSB(flMultiplier, 1, 1);
		int rectHeight = flHeight * flBoxes + 1;

		Draw::DrawRect(flX, flY, 4, size.h + 2, Color(80, 80, 80, 255));
		Draw::Outline(flX, flY, 4, size.h + 2, Color(0, 0, 0, 255));

		Draw::DrawRect(flX + 1, flY + size.h + (flMultiplier - rectHeight) + 1, 2, rectHeight, pEntity->IsDormant() ? Color(0, 0, 0, 140) : ColHealth);

		for (int i = 0; i < 10; i++) // 
			Draw::Line(flX, flY + i * flHeight2, flX + 4, flY + i * flHeight2, Color(0, 0, 0, 255));
	}
}

void CEsp::DrawArmor(EBox box, IClientEntity* pEntity)
{
	float armor = pEntity->ArmorValue();

	if (armor > 100)
		armor = 100;

	I::Surface->DrawSetColor(0, 0, 0, 150);
	if (visualconfig.iArmorLocation == 0)
	{
		I::Surface->DrawOutlinedRect(box.x + box.w + 1, box.y - 1, box.x + box.w + 5, box.y + box.h + 1);

		int height = armor * box.h / 100;

		if (armor > 0) {
			I::Surface->DrawSetColor(80, 135, 190, 255);
			I::Surface->DrawFilledRect(box.x + box.w + 2,
				box.y + box.h - height,
				box.x + box.w + 4,
				box.y + box.h);
		}
	}
	else if (visualconfig.iArmorLocation == 1)
	{
		if (visualconfig.iHealthBar)
		{
			I::Surface->DrawOutlinedRect(box.x - 1, box.y + box.h + 2, box.x + box.w + 1, box.y + box.h + 6);

			int width = armor * box.w / 100;

			if (armor > 0)
			{
				I::Surface->DrawSetColor(80, 135, 190, 255);
				I::Surface->DrawFilledRect(box.x, //x1
					box.y + box.h + 3, //y1
					box.x + width, //x2
					box.y + box.h + 5); //y2
			}
		}
		else
		{
			I::Surface->DrawOutlinedRect(box.x - 1, box.y + box.h + 7, box.x + box.w + 1, box.y + box.h + 11);

			int width = armor * box.w / 100;

			if (armor > 0)
			{
				I::Surface->DrawSetColor(80, 135, 190, 255);
				I::Surface->DrawFilledRect(box.x, //x1
					box.y + box.h + 8, //y1
					box.x + width, //x2
					box.y + box.h + 10); //y2
			}
		}
	}
}

void CEsp::DrawBoneTest(IClientEntity *pEntity)
{
	for (int i = 0; i < 127; i++)
	{
		Vector BoneLoc = pEntity->GetBonePos(i);
		Vector BoneScreen;
		if (Draw::WorldToScreen(BoneLoc, BoneScreen))
		{
			char buf[10];
			_itoa_s(i, buf, 10);
			Draw::Text(BoneScreen.x, BoneScreen.y, Color(255, 255, 255, 180), Draw::Fonts::ESP, buf);
		}
	}
}
void CEsp::DrawSkeleton(IClientEntity* pEntity)
{
	studiohdr_t* pStudioHdr = I::ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			if (Draw::WorldToScreen(vParent, sParent) && Draw::WorldToScreen(vChild, sChild))
			{
				Draw::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255, 255, 255, 255));
			}
		}
	}
}
void CEsp::DrawName(EBox box, IClientEntity* pEntity, player_info_t pinfo)
{
	
}

void CEsp::BulletTrace(IClientEntity* pEntity, Color color)
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	AngleVectors(*pEntity->GetEyeAngles(), &forward);
	filter.pSkip = pEntity;
	src3D = pEntity->GetBonePos(6) - Vector(0, 0, 0);
	dst3D = src3D + (forward * Menu::Window.VisualsTab.BulletTraceLength.GetValue());

	ray.Init(src3D, dst3D);

	if (!Draw::WorldToScreen(src3D, src) || !Draw::WorldToScreen(tr.endpos, dst))
		return;


	int red = Menu::Window.MiscTab.Menu_Red.GetValue();
	int green = Menu::Window.MiscTab.Menu_Green.GetValue();
	int blue = Menu::Window.MiscTab.Menu_Blue.GetValue();


	Draw::DrawLine(src.x, src.y, dst.x, dst.y, Color(red, green, blue, 225));
	Draw::DrawRect(dst.x - 3, dst.y - 3, 6, 6, Color(red, green, blue, 255));
}

void CEsp::DrawDistance(CEsp::EBox size, IClientEntity* pEntity)
{
	IClientEntity *pLocal = hackManager.pLocal();

	Vector vecOrigin = pEntity->GetOrigin();
	Vector vecOriginLocal = pLocal->GetOrigin();
	static RECT defSize = Draw::GetTextSize(Draw::Fonts::Default, "");

	char dist_to[32];
	sprintf_s(dist_to, "%.0f ft", DistanceTo(vecOriginLocal, vecOrigin));

	Draw::Text(size.x + size.w + 3, size.y + (0.6*(defSize.bottom + 28)), Color(255, 255, 255, 255), Draw::Fonts::ESP, dist_to);
}



void CEsp::DrawGlow()
{
	int GlowR = Menu::Window.ColorsTab.GlowR.GetValue();
	int GlowG = Menu::Window.ColorsTab.GlowG.GetValue();
	int GlowB = Menu::Window.ColorsTab.GlowB.GetValue();
	int GlowZ = Menu::Window.ColorsTab.GlowZ.GetValue();

	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	for (int i = 0; i < GlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		IClientEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{

		case 35:
			if (Menu::Window.VisualsTab.Glow.GetState() /*&& Entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CCSPlayer*/)
			{
				if (!Menu::Window.VisualsTab.Glow.GetState() && !(Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
					break;
				if (Menu::Window.VisualsTab.EnemyEspOnly.GetState() && (Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
					break;

				if (GameUtils::IsVisible(hackManager.pLocal(), Entity, 0))
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(Menu::Window.ColorsTab.GlowR.GetValue(), Menu::Window.ColorsTab.GlowG.GetValue(), Menu::Window.ColorsTab.GlowB.GetValue(), 200) : Color(Menu::Window.ColorsTab.GlowR.GetValue(), Menu::Window.ColorsTab.GlowG.GetValue(), Menu::Window.ColorsTab.GlowB.GetValue(), 200));
				}

				else
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(Menu::Window.ColorsTab.GlowR.GetValue(), Menu::Window.ColorsTab.GlowG.GetValue(), Menu::Window.ColorsTab.GlowB.GetValue(), 200) : Color(Menu::Window.ColorsTab.GlowR.GetValue(), Menu::Window.ColorsTab.GlowG.GetValue(), Menu::Window.ColorsTab.GlowB.GetValue(), 200));
				}
			}

		case 39:

		default:
			if (Menu::Window.VisualsTab.FiltersWeapons.GetState())
			{
				if (strstr(Entity->GetClientClass()->m_pNetworkName, "CWeapon"))
					glowEntity->set(Color(GlowR, GlowG, GlowB, 0));
			}
		}
	}
}

