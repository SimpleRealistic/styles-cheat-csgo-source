
#pragma once

#include "Hacks.h"

// All misc visuals (Crosshairs etc) go in this class

class CVisuals : public CHack
{
public:
	void Init();
	void Draw();
	void DrawRecoilCrosshair();
	void bighak();
	void SpreadCrosshair();
	void Move(CUserCmd *pCmd, bool& bSendPacket);
private:

};