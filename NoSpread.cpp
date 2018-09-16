#include "RageBot.h"
#include "RenderManager.h"
#include "Autowall.h"
#include "nospread.h"


CNoSpread NoSpread;

C_Random* Random = new C_Random;

void C_Random::SetSeed(int iSeed)
{
	m_idum = ((iSeed < 0) ? iSeed : -iSeed);
	m_iy = 0;
}

int C_Random::GenerateRandomNumber()
{
	int j;
	int k;

	if (m_idum <= 0 || !m_iy) {
		if (-(m_idum) < 1) m_idum = 1;
		else m_idum = -(m_idum);

		for (j = NTAB + 7; j >= 0; j--) {
			k = (m_idum) / IQ;
			m_idum = IA * (m_idum - k * IQ) - IR * k;
			if (m_idum < 0) m_idum += IM;
			if (j < NTAB) m_iv[j] = m_idum;
		}
		m_iy = m_iv[0];
	}
	k = (m_idum) / IQ;
	m_idum = IA * (m_idum - k * IQ) - IR * k;
	if (m_idum < 0) m_idum += IM;
	j = m_iy / NDIV;
	m_iy = m_iv[j];
	m_iv[j] = m_idum;

	return m_iy;
}

float C_Random::RandomFloat(float flLow, float flHigh)
{
	float fl = AM * (float)this->GenerateRandomNumber();
	if (fl > RNMX) fl = RNMX;
	return (fl * (flHigh - flLow)) + flLow;
}

void CNoSpread::CalcClient(Vector vSpreadVec, Vector ViewIn, Vector &ViewOut)
{
	Vector vecForward, vecRight, vecUp, vecSpreadDir;


	vecSpreadDir = vecForward + vecRight * -vSpreadVec.x + vecUp * -vSpreadVec.y;


}

void CNoSpread::CalcServer(Vector vSpreadVec, Vector ViewIn, Vector &vecSpreadDir)
{
	Vector vecViewForward, vecViewRight, vecViewUp;


	vecSpreadDir = vecViewForward + vecViewRight * vSpreadVec.x + vecViewUp * vSpreadVec.y;
}

void CNoSpread::GetSpreadVec(CUserCmd*pCmd, Vector &vSpreadVec)
{
	IClientEntity* pLocal = hackManager.pLocal();

	if (!pLocal)
		return;

	CBaseCombatWeapon* localWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!localWeapon)
		return;

	Vector vSpread;

	localWeapon->UpdateAccuracyPenalty();

	Random->SetSeed((pCmd->random_seed & 0xFF) + 1);

	float fRand1 = Random->RandomFloat(0.f, 1.f);
	float fRandPi1 = Random->RandomFloat(0.f, 2.f * (float)M_PI);
	float fRand2 = Random->RandomFloat(0.f, 1.f);
	float fRandPi2 = Random->RandomFloat(0.f, 2.f * (float)M_PI);
	int weapon = *localWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();

	if (weapon == 64 && pCmd->buttons & IN_ATTACK2)
	{
		fRand1 = 1.f - fRand1 * fRand1;
		fRand2 = 1.f - fRand2 * fRand2;
	}

	float fRandInaccuracy = fRand1 * localWeapon->GetInaccuracy();
	float fRandSpread = fRand2 * localWeapon->GetSpread();

	vSpreadVec.x = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
	vSpreadVec.y = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;

}
void CNoSpread::AngleVectors(Vector angles, Vector &f)
{
	float sp, sy, sr, cp, cy, cr;

	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	f[0] = cp * cy;
	f[1] = cp * sy;
	f[2] = -sp;
}

void CNoSpread::AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float angle;
	static float sr, sp, sy, cr, cp, cy;

	angle = angles[1] * (M_PI * 2 / 360);
	sy = sin(angle);
	cy = cos(angle);

	angle = angles[0] * (M_PI * 2 / 360);
	sp = sin(angle);
	cp = cos(angle);

	angle = angles[2] * (M_PI * 2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1 * sr*sp*cy + -1 * cr*-sy);
		right[1] = (-1 * sr*sp*sy + -1 * cr*cy);
		right[2] = -1 * sr*cp;
	}
	if (up)
	{
		up[0] = (cr*sp*cy + -sr*-sy);
		up[1] = (cr*sp*sy + -sr*cy);
		up[2] = cr*cp;
	}
}

void CNoSpread::VectorAngles(const float *forward, float *angles)
{
	static float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 90.0;
		else
			pitch = 270.0;
	}
	else
	{
		yaw = (float)(atan2(forward[1], forward[0]) * 180.0 / M_PI);
		if (yaw < 0)
			yaw += 360.0;
		tmp = (float)sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (float)(atan2(forward[2], tmp) * 180 / M_PI);
	}

	angles[0] = -pitch;
	angles[1] = yaw;
	angles[2] = 0;
}
long double VectorNormalize2(float *flAngles)
{
	long double ldRet = sqrt(flAngles[0] * flAngles[0] + flAngles[1] * flAngles[1] + flAngles[2] * flAngles[2]);

	if (ldRet != 0.0f)
	{
		flAngles[0] = flAngles[0] * (1.0f / (float)ldRet);
		flAngles[1] = flAngles[1] * (1.0f / (float)ldRet);
		flAngles[2] = flAngles[2] * (1.0f / (float)ldRet);
	}

	return ldRet;
}
void CNoSpread::CompensateInAccuracyNumeric(CUserCmd*cmd)
{
	Vector Aim, qAntiSpread;

	Vector vSpreadVec, vForward, vServerSpreadForward;

	Aim = cmd->viewangles;

	AngleVectors(Aim, vForward);

	GetSpreadVec(cmd, vSpreadVec);

	CalcClient(vSpreadVec, Aim, qAntiSpread);

	for (int i = 0; i <= 24; i++)
	{
		CalcServer(vSpreadVec, qAntiSpread, vServerSpreadForward);

		Vector qModifer;

		qModifer = Aim - qModifer;

		if (sqrt((qModifer.x * qModifer.x) + (qModifer.y * qModifer.y)) == 0)
		{
			break;
		}

		qAntiSpread = qAntiSpread + qModifer;

		cmd->viewangles = qAntiSpread;
	}

	cmd->viewangles -= PredictPunchAngles();
}
#define square( x ) ( x * x ) 

void CNoSpread::VectorAngles(const Vector &forward, const Vector &pseudoup, Vector &angles)
{
	Vector left;

	left = CrossProduct(pseudoup, forward);

	left.NormalizeInPlace();

	float xyDist = (forward[0] * forward[0] + forward[1] * forward[1]);

	if (xyDist > 0.001f)
	{
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

		float up_z = (left[1] * forward[0]) - (left[0] * forward[1]);

		angles[2] = RAD2DEG(atan2f(left[2], up_z));
	}
	else
	{
		angles[1] = RAD2DEG(atan2f(-left[0], left[1]));
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));
		angles[2] = 0;
	}
}

void CNoSpread::RollSpread(CBaseCombatWeapon*localWeap, int seed, CUserCmd*cmd, Vector& pflInAngles)
{
	//pflInAngles.x = AngleNormalize(pflInAngles.x);
	//pflInAngles.y = AngleNormalize(pflInAngles.y);

	localWeap->UpdateAccuracyPenalty();

	Random->SetSeed((seed & 0xFF) + 1);


	float fRand1 = Random->RandomFloat(0.f, 1.f);
	float fRandPi1 = Random->RandomFloat(0.f, 2.f * (float)M_PI);
	float fRand2 = Random->RandomFloat(0.f, 1.f);
	float fRandPi2 = Random->RandomFloat(0.f, 2.f * (float)M_PI);

	int weapon = *localWeap->m_AttributeManager()->m_Item()->ItemDefinitionIndex();

	if (weapon == 64 && cmd->buttons & IN_ATTACK2)
	{
		fRand1 = 1.f - fRand1 * fRand1;
		fRand2 = 1.f - fRand2 * fRand2;
	}

	float fRandInaccuracy = fRand1 * localWeap->GetInaccuracy();
	float fRandSpread = fRand2 * localWeap->GetSpread();

	Vector vForward, vRight, vUp, vDir;
	Vector vView, vSpread, flIdentity[3];
	float flRoll, flCross;

	vSpread.x = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
	vSpread.y = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;

	Vector qViewAngles = pflInAngles;

	vSpread[0] = -vSpread[0];
	vSpread[1] = -vSpread[1];



	vDir[0] = vForward[0] + (vRight[0] * vSpread[0]) + (vUp[0] * vSpread[1]);
	vDir[1] = vForward[1] + (vRight[1] * vSpread[0]) + (vUp[1] * vSpread[1]);
	vDir[2] = vForward[2] + (vRight[2] * vSpread[0]) + (vUp[2] * vSpread[1]);


	flIdentity[2][0] = 1.0f;
	flIdentity[2][1] = -vSpread[0];
	flIdentity[2][2] = vSpread[1];


	flIdentity[0][0] = 0.0f;
	flIdentity[0][1] = -vSpread[0];
	flIdentity[0][2] = (1.0f / vSpread[1]) + (1.0f / flIdentity[2][2]) + vSpread[1];

	if (vSpread[0] > 0.0f && vSpread[1] < 0.0f)
	{
		if (flIdentity[0][1] < 0.0f)
			flIdentity[0][1] = -flIdentity[0][1];
	}
	else if (vSpread[0] < 0.0f && vSpread[1] < 0.0f)
	{
		if (flIdentity[0][1] > 0.0f)
			flIdentity[0][1] = -flIdentity[0][1];
	}

	if (flIdentity[0][2] < 0.0f)
		flIdentity[0][2] = -flIdentity[0][2];


	flIdentity[1] = CrossProduct(flIdentity[0], flIdentity[2]);


	flCross = (flIdentity[1][1] * flIdentity[2][0]) - (flIdentity[1][0] * flIdentity[2][1]);
}