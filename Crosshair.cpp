#include "Crosshair.h"

Crosshair crosshair;

void Crosshair::Draw()
{
	if (visualconfig.bCrosshair)
	{
		if (visualconfig.iCrosshairType == 0)
		{
			int screenW, screenH;
			I::Engine->GetScreenSize(screenW, screenH);

			RECT View = Draw::GetViewport();
			int MidX = View.right / 2;
			int MidY = View.bottom / 2;
			Draw::Line(MidX - 8, MidY, MidX - 4, MidY, Color(255, 0, 0, 200));
			Draw::Line(MidX, MidY - 8, MidX, MidY - 4, Color(255, 0, 0, 200));
			Draw::Line(MidX + 8, MidY, MidX + 4, MidY, Color(255, 255, 255, 255));
			Draw::Line(MidX, MidY + 8, MidX, MidY + 4, Color(255, 255, 255, 255));
		}
		else if (visualconfig.iCrosshairType == 1)
		{
			int screenW, screenH;
			I::Engine->GetScreenSize(screenW, screenH);

			int crX = screenW / 2, crY = screenH / 2;
			int dy = screenH / globals.fov;
			int dx = screenW / globals.fov;
			int drX;
			int drY;

			if (visualconfig.bNoVisualRecoil)
			{
				Vector AimPunch = hackManager.pLocal()->localPlayerExclusive()->GetAimPunchAngle();
				drX = crX - (int)(dx * (AimPunch.y * 2));
				drY = crY + (int)(dy * (AimPunch.x * 2));
			}
			else
			{
				Vector AimPunch = hackManager.pLocal()->localPlayerExclusive()->GetAimPunchAngle();
				drX = crX - (int)(dx * (((AimPunch.y * 2.f) * 0.45f) + AimPunch.y));
				drY = crY + (int)(dy * (((AimPunch.x * 2.f) * 0.45f) + AimPunch.x));
			}

			Draw::DrawCircle(drX, drY, 5, 30, globals.forecolor);
		}
	}
}