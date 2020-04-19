#pragma once

#include "lich.h"
#include "Challenge.h"

namespace lich
{
	// [Todo]: Change source and sourceName to hostSteamID and hostName
	class ChallengeRequest
	{
	public:
		ChallengeType challengeType;
		CSteamID source;
		std::wstring sourceName;
		ChallengeRequest(ChallengeType cT, CSteamID s, std::wstring sN) :
			challengeType(cT), source(s), sourceName(sN), sentPacket(false) {}
		std::wstring PrintRequest();
		void AcceptRequest(std::wstring name);
		void DeclineRequest();

	private:
		bool sentPacket;
	};
}