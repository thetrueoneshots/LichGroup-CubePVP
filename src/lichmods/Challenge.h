#pragma once

#include "lich.h"
#include "ChallengeClient.h"
#include "ChallengeHost.h"

namespace lich 
{
	enum class ChallengeInterface : unsigned int 
	{
		Host,
		Client
	};

	// [Todo]:	Make a ChallengeBuff class with a ChallengeBuffType enum
	enum class ChallengeBuff : unsigned int
	{
		Freeze,
		UnFreeze,
	};

	class Challenge
	{
	public:
		Challenge() : type(ChallengeInterface::Client), client(ChallengeClient()) {}
		void SetClientChallenge(ChallengeClient obj);
		void SetHostChallenge(ChallengeHost obj);
		bool HandlePacket(BytesIO* bytesio, CSteamID steamIDRemote);
		bool OnGameTick(cube::Creature* creature);
		bool AbortChallenge();
		ChallengeInterface GetInterfaceType();
	private:
		ChallengeInterface type;
		ChallengeClient client;
		ChallengeHost host;
	};
}