#pragma once

#include <vector>
#include "lich.h"
#include "ChallengeAction.h"

namespace lich
{
	class InvitedPlayer
	{
	public:
		std::wstring name;
		CSteamID steamID;
		bool accepted;
		InvitedPlayer(std::wstring n) : name(n), steamID(CSteamID()), accepted(false) {}
	};

	class ChallengeHost
	{
	public:
		std::vector<InvitedPlayer> invited;
		ChallengeState state;
		ChallengeType type;
		std::vector<ChallengeAction> actions;
		std::vector<LongVector3> positions;
		ChallengeHost() : state(ChallengeState::Idle), type(ChallengeType::NoChallenge) {}
		ChallengeHost(ChallengeType t) : state(ChallengeState::Idle), type(t) {}

		bool HandlePacket(BytesIO* bytesio, CSteamID steamIDRemote);
		bool OnGameTick(cube::Creature* creature);
		bool AbortChallenge();
	private:
		//int PlayersAcceptedCount();
		void PlayerAcceptedCount(int* cnt);
		void HandleAcceptPacket(BytesIO* bytesio, CSteamID steamIDRemote);
		void HandleDeclinePacket(BytesIO* bytesio, CSteamID steamIDRemote);
		void StartChallenge(cube::Creature* creature);
	};
}