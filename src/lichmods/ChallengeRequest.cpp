#include "lich.h"
#include "ChallengeRequest.h"

// [Todo]:	Better format for printing the request
std::wstring lich::ChallengeRequest::PrintRequest()
{
	wchar_t buffer[100];
	swprintf(buffer, 100, L"Challenge type %d by %ls\n", challengeType, (wchar_t*)sourceName.c_str());
	return std::wstring(buffer);
}

// [Todo]:	Implement function
void lich::ChallengeRequest::AcceptRequest(std::wstring name)
{
	// Send a packet with the accept packet
	wchar_t* playerName = (wchar_t*)name.c_str();
	BytesIO bytesio;

	bytesio.Write<lich::PacketID>(lich::PacketID::Challenge);
	bytesio.Write<lich::ChallengeInterface>(lich::ChallengeInterface::Host);
	bytesio.Write<lich::ChallengePacketID>(lich::ChallengePacketID::AcceptChallenge);
	bytesio.Write<unsigned int>(wcslen(playerName));
	for (int i = 0; i < wcslen(playerName); i++) {
		bytesio.Write<wchar_t>(playerName[i]);
	}
	lich::PrintMessage(L"I did send an accept packet lol\n");
	lich::GetSteamNetwork()->SendP2PPacket(source, bytesio.Data(), bytesio.Size(), k_EP2PSendReliable, 2);
}
// [Todo]:	Implement function
void lich::ChallengeRequest::DeclineRequest()
{
	// Send a packet with the decline packet
}
