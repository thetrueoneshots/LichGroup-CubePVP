#pragma once

#include "../../cwmods/cwsdk.h"
#include <algorithm>
#include <cctype>

#define DEBUG_MODE true

namespace lich {
	void PrintMessage(wchar_t* text);
	void DebugMessage(wchar_t* text);

	ISteamNetworking* GetSteamNetwork();

	std::wstring getLowerCaseName(cube::Creature* creature);
	std::wstring toLowerCase(std::wstring string);
	
	void teleport(cube::Creature* creature, LongVector3 target);
	void teleport(cube::Creature* creature, float target[3]);
	void teleport(cube::Creature* creature, long long target[3]);

	enum class ChallengeType : unsigned int
	{
		NoChallenge,
		OneVersusOne,
	};

	enum class PacketID : unsigned int 
	{
		ChallengeRequest,
		Challenge,
	};

	enum class ChallengePacketID : unsigned int 
	{
		AcceptChallenge,
		DeclineChallenge,
	};

	enum class ChallengeState : unsigned int 
	{
		InChallenge,
		SentChallenge,
		Idle
	};
}