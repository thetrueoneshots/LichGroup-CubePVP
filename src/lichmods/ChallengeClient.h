#pragma once


#include <vector>
#include "lich.h"
#include "ChallengeAction.h"
#include "ChallengeRule.h"

namespace lich {
	class ChallengeClient {
	public:
		std::wstring hostName;
		CSteamID hostSteamID;
		ChallengeType challengeType;
		std::vector<LongVector3> positions;
		std::vector<ChallengeAction> actions;
		std::vector<ChallengeRule> rules;
		ChallengeState state;
		ChallengeClient() : 
			hostName(std::wstring()), 
			hostSteamID(CSteamID()), 
			challengeType(ChallengeType::NoChallenge), 
			state(ChallengeState::Idle) {}
		ChallengeClient(std::wstring n, CSteamID id, ChallengeType nr) : 
			hostName(n), 
			hostSteamID(id), 
			challengeType(nr),
			state(ChallengeState::InChallenge) {
			rules.push_back(ChallengeRule(ChallengeRuleType::OnDeath));
		}
		bool HandlePacket(BytesIO* bytesio, CSteamID steamIDRemote);
		bool OnGameTick(cube::Creature* creature);
	};
}