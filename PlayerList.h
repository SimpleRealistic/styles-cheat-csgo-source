#pragma once

#include "Interfaces.h"

class PlayerList
{
public:
	int index = 0;
	unsigned long iUserId = 0;
	bool bFriend = false;
	int iHitbox = 0;
	char cName[128];
	int iResolve = 0;
	//std::vector<YawStorage> yawDatabase;
	int expireTime = 30;
	float lastUpdatedTick = 0;
	int lastDifference = 0;
	bool lowerBodyInitialized = false;
	int lastLowerBodyUpdate = 3654365346;
	bool shouldResetStorage = false;
	std::array<float, 24>* poseParams;
	std::array<float, 24>* oldPoseParams;
public:
	PlayerList(PlayerList _list, char _name[128])
	{
		this->index = _list.index;
		this->iUserId = _list.iUserId;
		strcpy(this->cName, _name);
		this->iHitbox = _list.iHitbox;
		this->bFriend = _list.bFriend;
		this->iResolve = _list.iResolve;
		//this->yawDatabase = _list.yawDatabase;
		this->expireTime = _list.expireTime;
		this->lowerBodyInitialized = _list.lowerBodyInitialized;
		this->lastUpdatedTick = _list.lastUpdatedTick;
		this->lastDifference = _list.lastDifference;
		this->lastLowerBodyUpdate = _list.lastLowerBodyUpdate;
		this->shouldResetStorage = _list.shouldResetStorage;
	}
	PlayerList(int _index, unsigned long _userID, char _name[128], int _hitbox, bool _friend, int _resolve)
	{
		this->index = _index;
		this->iUserId = _userID;
		strcpy(this->cName, _name);
		this->iHitbox = _hitbox;
		this->bFriend = _friend;
		this->iResolve = _resolve;
	}
	friend bool operator==(const PlayerList& p1, const PlayerList& p2)
	{
		return p1.iUserId == p2.iUserId;
	}
	friend bool operator==(const PlayerList& p1, const unsigned long& p2)
	{
		return p1.iUserId == p2;
	}
};