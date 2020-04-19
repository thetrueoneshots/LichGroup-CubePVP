#include "ChallengeHandler.h"

bool lich::ChallengeHandler::AcceptRequest(int i)
{
	ChallengeRequest* rq = challengeRequestList.AcceptRequest(i, player.name);

	if (rq == nullptr)
		return false;
	// Do stuff with the Challenge Request
	challenge.SetClientChallenge(lich::ChallengeClient(rq->sourceName, rq->source, rq->challengeType));
	// Decline all other requests
	challengeRequestList.DeclineAllRequests();
	return true;
}

std::wstring lich::ChallengeHandler::PrintRequests()
{
	return challengeRequestList.PrintRequests();
}
bool lich::ChallengeHandler::HandlePacket(BytesIO* bytesio, CSteamID steamIDRemote)
{
	lich::PacketID packetType = bytesio->Read<lich::PacketID>();
	if (packetType == lich::PacketID::ChallengeRequest) {
		lich::DebugMessage(L"Request packet received\n");
		challengeRequestList.HandlePacket(bytesio, steamIDRemote, player.playerPointer);
		return true;
	}
	else if (packetType == lich::PacketID::Challenge) {
		lich::DebugMessage(L"Challenge packet received\n");
		challenge.HandlePacket(bytesio, steamIDRemote);
		return true;
	}
	return false;
}
// [Todo]: Implement method
bool lich::ChallengeHandler::HasChallenge()
{
	return false;
}

bool lich::ChallengeHandler::CreateChallenge(ChallengeType type, std::vector<std::wstring> inv)
{
	ChallengeHost host = ChallengeHost(type);
	wchar_t buffer[100];
	swprintf_s(buffer, 100, L"[ChallengeHandler::CreateChallenge]\n    Invited size: %d \n", inv.size());
	lich::DebugMessage(buffer);
	for (int i = 0; i < inv.size(); i++) {
		host.invited.push_back(InvitedPlayer(inv[i]));
	}
	host.state = ChallengeState::SentChallenge;
	challenge.SetHostChallenge(host);
	return false;
}

lich::Challenge* lich::ChallengeHandler::GetChallenge()
{
	return &challenge;
}

void lich::ChallengeHandler::SetPlayerPointer(cube::Creature* ptr)
{
	player.SetPlayerPointer(ptr);
}

bool lich::ChallengeHandler::OnGameTick()
{
	return challenge.OnGameTick(player.playerPointer);
}

bool lich::ChallengeHandler::AbortChallenge()
{
	challenge.AbortChallenge();
	return false;
}
