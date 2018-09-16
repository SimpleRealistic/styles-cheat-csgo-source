#include "NightMode.h"

NightMode nightmode;

void NightMode::SetModulations()
{
	static bool bPerfomed = false, bLastSetting;

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
	{
		if (bPerfomed) {
			materials_props.clear();
			materials_world.clear();
			bPerfomed = false;
		}
		return;
	}

	if (!hackManager.pLocal())
	{
		if (bPerfomed)
			bPerfomed = false;
		return;
	}

	if (bLastSetting != visualconfig.bNightMode)
	{
		bLastSetting = visualconfig.bNightMode;
		bPerfomed = false;
	}

	if (!bPerfomed)
	{

		static ConVar* r_DrawSpecificStaticProp = I::CVar->FindVar("r_DrawSpecificStaticProp");
		r_DrawSpecificStaticProp->nFlags &= ~FCVAR_CHEAT;
		r_DrawSpecificStaticProp->SetValue(visualconfig.bNightMode ? 0 : 1);

		for (IMaterial* pMaterial : materials_world) {
			if (visualconfig.bNightMode)
				pMaterial->ColorModulate(0.2f, 0.2f, 0.2f);
			else
				pMaterial->ColorModulate(1.f, 1.f, 1.f);
			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_FLAT, visualconfig.bNightMode ? true : true);
		}

		for (IMaterial* pMaterial : materials_props) {
			if (visualconfig.bNightMode)
				pMaterial->ColorModulate(0.2f, 0.2f, 0.2f);
			else
				pMaterial->ColorModulate(1.f, 1.f, 1.f);
			pMaterial->SetMaterialVarFlag(MATERIAL_VAR_FLAT, visualconfig.bNightMode ? true : true);
		}
		bPerfomed = true;
	}
}

void NightMode::SetSkybox()
{
	static bool bPerfomed = false, bLastSetting;

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
	{
		if (bPerfomed) {
			bPerfomed = false;
		}
		return;
	}

	if (!hackManager.pLocal())
	{
		if (bPerfomed)
			bPerfomed = false;
		return;
	}

	if (bLastSetting != visualconfig.bNightMode)
	{
		bLastSetting = visualconfig.bNightMode;
		bPerfomed = false;
	}

	if (!bPerfomed)
	{
		static ConVar* sv_skyname = I::CVar->FindVar("sv_skyname");
		sv_skyname->nFlags &= ~FCVAR_CHEAT;

		if (visualconfig.bNightMode)
			sv_skyname->SetValueChar("sky_csgo_night02");
		else
			sv_skyname->SetValueChar("sky_dust");

		bPerfomed = true;
	}
}