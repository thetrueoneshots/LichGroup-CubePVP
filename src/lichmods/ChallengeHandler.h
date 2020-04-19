#pragma once

//#include "lich.h"
#include "ChallengeRequestList.h"
#include "Challenge.h"
#include "Player.h"

namespace lich 
{
	class ChallengeHandler 
	{
	public:
		bool AcceptRequest(int i);
		bool DeclineRequest(int i);
		bool DeclineAllRequests();
		std::wstring PrintRequests();
		int GetRequestCount();
		bool HandlePacket(BytesIO* bytesio, CSteamID steamIDRemote);
		bool HasChallenge();
		bool CreateChallenge(ChallengeType type, std::vector<std::wstring> inv);
		lich::Challenge* GetChallenge();
		void SetPlayerPointer(cube::Creature* ptr);
		bool OnGameTick();
		bool AbortChallenge();
	private:
		lich::ChallengeRequestList challengeRequestList;
		lich::Challenge challenge;
		lich::Player player;
	};
}
