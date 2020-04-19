#pragma once

#include <vector>
#include "ChallengeRequest.h"

namespace lich
{
	class ChallengeRequestList
	{
	public:
		lich::ChallengeRequest* AcceptRequest(int i, std::wstring name);
		bool DeclineRequest(int i);
		bool DeclineAllRequests();
		std::wstring PrintRequests();
		int GetRequestCount();
		bool HandlePacket(BytesIO* bytesio, CSteamID source, cube::Creature* ptr);
	private:
		std::vector<lich::ChallengeRequest> challengeRequests;
	};
}
