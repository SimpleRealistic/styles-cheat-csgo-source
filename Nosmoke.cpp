/*
Syn's AyyWare Framework 2015
*/

#include "Interfaces.h"
#include "Menu.h"
#include "Nosmoke.h"




RecvVarProxyFn oSmokeEffectTickBegin = NULL;
RecvVarProxyFn oFlashMaxAlpha = NULL;
RecvVarProxyFn oDidSmokeEffect = NULL;
// Credits to Valve and Shad0w


//void RecvProxy_FlashMaxAlpha(CRecvProxyData *pData, void *pStruct, void *pOut)
//{
//	float *value = &pData->m_Value.m_Float;
//
//	if (Menu::Window.VisualsTab2.playeresp.GetState())
//	{
//		*value = 0.0f;
//	}
//	else
//	{
//		*value = 255.0f;
//	}
//
//	oFlashMaxAlpha(pData, pStruct, pOut);
//}

void RecvProxy_SmokeEffectTickBegin(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float *value = &pData->m_Value.m_Float;

	
	{
		IClientEntity *entity = (IClientEntity*)pStruct;

		if (entity)
		{
			//*value = Interfaces::Globals->tickcount - 60000;
			entity->GetOrigin() = Vector(10000, 10000, 10000);
		}
	}

	oSmokeEffectTickBegin(pData, pStruct, pOut);
}

void RecvProxy_DidSmokeEffect(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	int *value = (int*)&pData->m_Value.m_Int;

	if (Menu::Window.VisualsTab.OtherNoSmoke.GetState())
	{
		IClientEntity *entity = (IClientEntity*)pStruct;
		*value = 0;
	}

	oDidSmokeEffect(pData, pStruct, pOut);
}

void ApplyNetVarsHooks()
{
	ClientClass *pClass = I::Client->GetAllClasses();

	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;

		for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
		{
			RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
			const char *name = pProp->m_pVarName;

			/*if (!strcmp(pszName, "DT_BaseViewModel"))
			{
			// Knives
			if (!strcmp(name, "m_nModelIndex"))
			{
			oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
			pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
			}
			}
			else */if (!strcmp(pszName, "DT_SmokeGrenadeProjectile"))
			{
				if (!strcmp(name, "m_nSmokeEffectTickBegin"))
				{
					oSmokeEffectTickBegin = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = RecvProxy_SmokeEffectTickBegin;
				}
				else if (!strcmp(name, "m_bDidSmokeEffect"))
				{
					oDidSmokeEffect = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = RecvProxy_DidSmokeEffect;
				}
			}
			
		}

		pClass = pClass->m_pNext;
	}
}

void RemoveNetVarsHooks()
{
	ClientClass *pClass = I::Client->GetAllClasses();

	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;

		for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
		{
			RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
			const char *name = pProp->m_pVarName;

			/*if (!strcmp(pszName, "DT_BaseViewModel"))
			{
			// Knives
			if (!strcmp(name, "m_nModelIndex"))
			{
			pProp->m_ProxyFn = oRecvnModelIndex;
			}
			}
			else */if (!strcmp(pszName, "DT_SmokeGrenadeProjectile"))
			{
				if (!strcmp(name, "m_nSmokeEffectTickBegin"))
				{
					pProp->m_ProxyFn = oSmokeEffectTickBegin;
				}
				else if (!strcmp(name, "m_bDidSmokeEffect"))
				{
					pProp->m_ProxyFn = oDidSmokeEffect;
				}
			}
			else if (!strcmp(pszName, "DT_CSPlayer"))
			{
				if (!strcmp(name, "m_flFlashMaxAlpha"))
				{
					pProp->m_ProxyFn = oFlashMaxAlpha;
				}
			}
		}

		pClass = pClass->m_pNext;
	}
}

// Shad0ws Yaw fix
// (FIX ME UP LATER)
void FixY(const CRecvProxyData *pData, void *pStruct, void *pOut)
{


	static Vector vLast[65];
	static bool bShotLastTime[65];
	static bool bJitterFix[65];

	float *flPitch = (float*)((DWORD)pOut - 4);
	float flYaw = pData->m_Value.m_Float;
	bool bHasAA;
	bool bSpinbot;


	*(float*)(pOut) = flYaw;
}

//// Simple fix for some Fake-Downs
//void FixX(const CRecvProxyData* pData, void* pStruct, void* pOut) // Clamp other player angles to fix fakedown or lisp
//{
//	float flPitch = pData->m_Value.m_Float;
//	static float time = 1;
//	time++;
//	if (Menu::Window.RageBotTab.AccuracyResolverPitch.GetState())
//	{
//		if (time < 180)
//		{
//			flPitch = time;
//		}
//		else
//		{
//			time = 1;
//		}
//	}
//	*(float*)pOut = flPitch;
//}


RecvVarProxyFn oRecvnModelIndex;



void ApplyAAAHooks()
{
	ClientClass *pClass = I::Client->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
		if (!strcmp(pszName, "DT_CSPlayer"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				// Pitch Fix
				

				// Yaw Fix
				if (!strcmp(name, "m_angEyeAngles[1]"))
				{
					U::Log("Yaw Fix Applied");
					pProp->m_ProxyFn = FixY;
				}
			}
		}
		else if (!strcmp(pszName, "DT_BaseViewModel"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				
			}
		}
		pClass = pClass->m_pNext;
	}
}