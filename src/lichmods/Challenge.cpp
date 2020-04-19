#include "Challenge.h"

void lich::Challenge::SetClientChallenge(ChallengeClient obj)
{
	type = ChallengeInterface::Client;
	client = obj;
}

void lich::Challenge::SetHostChallenge(ChallengeHost obj)
{
	type = ChallengeInterface::Host;
	host = obj;
}

bool lich::Challenge::HandlePacket(BytesIO* bytesio, CSteamID steamIDRemote)
{
	lich::DebugMessage(L"[Challenge::HandlePacket] Start \n");

	// [Bug]: Probably the place where the bug is.
	lich::ChallengeInterface ch = bytesio->Read<lich::ChallengeInterface>();
	if (type != ch)
		return false;

	lich::DebugMessage(L"[Challenge::HandlePacket]: Correct Interface \n");

	switch (ch) 
	{
	case ChallengeInterface::Host:
		lich::DebugMessage(L"[Challenge::HandlePacket]: Host forward\n");
		return host.HandlePacket(bytesio, steamIDRemote);
		break;
	case ChallengeInterface::Client: default:
		lich::DebugMessage(L"[Challenge::HandlePacket]: Client forward\n");
		return client.HandlePacket(bytesio, steamIDRemote);
		break;
	}
	return true;
}

bool lich::Challenge::OnGameTick(cube::Creature* creature)
{
	switch (type)
	{
	case ChallengeInterface::Host:
		host.OnGameTick(creature);
	break;
	case ChallengeInterface::Client:
	default:
		client.OnGameTick(creature);
		break;
	}
	return false;
}

bool lich::Challenge::AbortChallenge()
{
	switch (type) 
	{
	case ChallengeInterface::Client:
		lich::PrintMessage(L"Only the host can abort the challenge\n");
		break;
	case ChallengeInterface::Host:
		lich::PrintMessage(L"Host abort!\n");
		host.AbortChallenge();
		break;
	default:
		break;
	}
	return false;
}

lich::ChallengeInterface lich::Challenge::GetInterfaceType()
{
	return type;
}
