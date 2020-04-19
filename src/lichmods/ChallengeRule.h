#pragma once

#include "lich.h"

namespace lich
{
	enum class ChallengeRuleType : unsigned int
	{
		OnDeath,
		OnHit,
	};

	class ChallengeRule
	{
	public:
		LongVector3 position;
		ChallengeRuleType type;
		ChallengeRule(ChallengeRuleType t) : type(t) {}
		ChallengeRule(ChallengeRuleType t, LongVector3 p) : position(p), type(t) {}
		boolean ExecuteRule(cube::Creature* creature, std::vector<LongVector3>* positions);
	};
}