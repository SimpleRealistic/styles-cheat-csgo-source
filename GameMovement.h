#pragma once

#include "SDK.h"

class IGameMovement
{
public:
	virtual			~IGameMovement(void) {}

	virtual void	ProcessMovement(IClientEntity *pPlayer, void *pMove) = 0;
	virtual void	Reset(void) = 0;
	virtual void	StartTrackPredictionErrors(IClientEntity *pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(IClientEntity *pPlayer) = 0;
	virtual void	DiffPrint(char const *fmt, ...) = 0;

	virtual Vector const&	GetPlayerMins(bool ducked) const = 0;
	virtual Vector const&	GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;

	virtual bool			IsMovingPlayerStuck(void) const = 0;
	virtual IClientEntity*	GetMovingPlayer(void) const = 0;
	virtual void			UnblockPusher(IClientEntity* pPlayer, IClientEntity *pPusher) = 0;

	virtual void SetupMovementBounds(void *pMove) = 0;
};