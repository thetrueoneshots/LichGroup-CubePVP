#pragma once

#include "lich.h"

namespace lich 
{
	enum class ChallengeActionType : unsigned int 
	{
		Teleport,
		TeleportBack,
		PvPOn,
		PvPOff,
		RecoverHP,
		AbortChallenge,
	};

	class ChallengeAction
	{
	public:
		LongVector3 position;
		ChallengeActionType type;
		ChallengeAction(ChallengeActionType t) : type(t) {}
		ChallengeAction(ChallengeActionType t, LongVector3 p) : position(p), type(t) {}
		void ExecuteAction(cube::Creature* creature, std::vector<LongVector3>* positions);
	};
}