
#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "Globals.h"
#include "hooks.h"
#include "Vector.h"

#define MakePtr( Type, dwBase, dwOffset ) ( ( Type )( DWORD( dwBase ) + (DWORD)( dwOffset ) ) )
#define M_RADPI 57.295779513082f
#define square( x ) ( x * x )

// Any init here
void CVisuals::Init()
{
	// Idk
}

// Don't really need to do anything in here
void CVisuals::Move(CUserCmd *pCmd, bool& bSendPacket) {}

// Main ESP Drawing loop
void CVisuals::Draw()
{

	//// Crosshair
	//if (Menu::Window.VisualsTab.OtherCrosshair.GetState())
	//	DrawCrosshair();

	// Recoil Crosshair
	if (Menu::Window.VisualsTab.SpreadCrosshair.GetState())
		SpreadCrosshair();
}

//// Draw a basic crosshair
//void CVisuals::DrawCrosshair()
//{
//	// Crosshair
//	RECT View = Draw::GetViewport();
//	int MidX = View.right / 2;
//	int MidY = View.bottom / 2;
//	Draw::Line(MidX - 8, MidY - 8, MidX + 8, MidY + 8, Color(0, 0, 0, 200));
//	Draw::Line(MidX + 8, MidY - 8, MidX - 8, MidY + 8, Color(0, 0, 0, 200));
//	Draw::Line(MidX - 4, MidY - 4, MidX + 4, MidY + 4, Color(0, 255, 0, 255));
//	Draw::Line(MidX + 4, MidY - 4, MidX - 4, MidY + 4, Color(0, 255, 0, 255));
//}


void CVisuals::SpreadCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	IClientEntity* WeaponEnt = I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!hackManager.pLocal()->IsAlive())
		return;

	if (!GameUtils::IsBallisticWeapon(pWeapon))
		return;

	if (pWeapon == nullptr)  // Yeah fuck off with that shit
		return;

	int xs;
	int ys;
	I::Engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;

	auto accuracy = pWeapon->GetInaccuracy() * 550.f; //3000

	Draw::Outline(xs - (accuracy / 2) + 1, ys - (accuracy / 2) + 1, accuracy, accuracy, Color(255, 255, 255, 0));

	if (pLocal->IsAlive())
	{
		if (pWeapon)
		{
			float inaccuracy = pWeapon->GetInaccuracy() * 1000;
			char buffer4[64];
			sprintf_s(buffer4, "bullet spread:  %f", inaccuracy);
			Draw::Text(xs + accuracy + 4, ys, Color(255, 255, 255, 0), Draw::Fonts::ESP, buffer4);
		}
	}
	else
	{
		Draw::Text(10, 70, Color(255, 255, 255, 0), Draw::Fonts::ESP, "bullet spread: --");
	}

	Draw::DrawCircle(xs, ys, accuracy, accuracy, Color(255, 255, 255, 150));
}

// https://steamcommunity.com/id/simplerealistic/ (im very sexy btw feel free to add me) https://www.youtube.com/channel/UCvuWYGfnILTnsKcu5FxYJPw