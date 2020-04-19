#include "ChallengeClient.h"

bool lich::ChallengeClient::HandlePacket(BytesIO* bytesio, CSteamID steamIDRemote)
{
	if (hostSteamID.ConvertToUint64() != steamIDRemote.ConvertToUint64())
		return false;

	unsigned int cnt = bytesio->Read<unsigned int>();
	for (int i = 0; i < cnt; i++) {
		lich::ChallengeActionType type = bytesio->Read<lich::ChallengeActionType>();
		if (type == lich::ChallengeActionType::Teleport) {
			long long x = bytesio->Read<long long>();
			long long y = bytesio->Read<long long>();
			long long z = bytesio->Read<long long>();
			LongVector3 position = LongVector3(x, y, z);
			actions.push_back(lich::ChallengeAction(type, position));
		}
		else {
			actions.push_back(lich::ChallengeAction(type));
		}
	}

	return true;
}

bool lich::ChallengeClient::OnGameTick(cube::Creature* creature)
{
	if (state == ChallengeState::Idle) return false;
	for (int i = 0; i < rules.size(); i++) {
		if (rules[i].ExecuteRule(creature, &positions)) {
			// [Todo]:	Handle rules

			// [Todo]:	Add a hook that notifies the challenge if an [OnHit / OnUsePotion e.t.c. is used]
			// [Todo]:	Add hooks that depending on used stuff disable and enable things.
		}
	}


	for (int i = 0; i < actions.size(); i++) {
		if (actions[i].type == ChallengeActionType::AbortChallenge) {
			actions.push_back(ChallengeAction(ChallengeActionType::TeleportBack));
			actions.push_back(ChallengeAction(ChallengeActionType::RecoverHP));
			actions.push_back(ChallengeAction(ChallengeActionType::PvPOff));
			state = ChallengeState::Idle;
		}
		actions[i].ExecuteAction(creature, &positions);
	}
	actions.clear();
	return actions.size() == 0;
}
