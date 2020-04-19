#include "ChallengeHost.h"
#include "Challenge.h"

long long posOne[3] = { -304600797, 220564195, 6296698 };
long long posTwo[3] = { -304614591, 212580979, 6296698 };

bool lich::ChallengeHost::HandlePacket(BytesIO* bytesio, CSteamID steamIDRemote)
{
	lich::DebugMessage(L"[ChallengeHost::HandlePacket]: Start \n");
	lich::ChallengePacketID id = bytesio->Read<lich::ChallengePacketID>();
	switch (id)
	{
	case lich::ChallengePacketID::AcceptChallenge:
		lich::DebugMessage(L"[ChallengeHost::HandlePacket]: AcceptChallenge packet received\n");
		HandleAcceptPacket(bytesio, steamIDRemote);
		break;
	case lich::ChallengePacketID::DeclineChallenge:
		lich::DebugMessage(L"[ChallengeHost::HandlePacket]: DeclineChallenge packet received\n");
		HandleDeclinePacket(bytesio, steamIDRemote);
		break;
	default:
		lich::PrintMessage(L"Could not recognise received package type.\n");
		break;
	}
	return false;
}

bool lich::ChallengeHost::OnGameTick(cube::Creature* creature)
{
	if (state == ChallengeState::SentChallenge) {
		int x = 0;
		PlayerAcceptedCount(&x);
		if (x == invited.size()) {
			wchar_t buffer[100];
			swprintf_s(buffer, 100, L"Fucking %d accepts \n", x);
			lich::DebugMessage(buffer);
			StartChallenge(creature);
		}
	} 
	
	if (state == ChallengeState::Idle) return false;
	
	for (int i = 0; i < actions.size(); i++) {
		if (actions[i].type == ChallengeActionType::AbortChallenge) {
			actions.push_back(ChallengeAction(ChallengeActionType::TeleportBack));
			actions.push_back(ChallengeAction(ChallengeActionType::RecoverHP));
			actions.push_back(ChallengeAction(ChallengeActionType::PvPOff));
			state = ChallengeState::Idle;
			invited.clear();
			type = ChallengeType::NoChallenge;
		}
		actions[i].ExecuteAction(creature, &positions);
	}
	actions.clear();
	return actions.size() == 0;
}

bool lich::ChallengeHost::AbortChallenge()
{
	if (state != ChallengeState::InChallenge)
		return false;
	lich::PrintMessage(L"Abort->In challenge\n");
	actions.push_back(ChallengeAction(ChallengeActionType::AbortChallenge));

	BytesIO bytesio;
	bytesio.Write<lich::PacketID>(PacketID::Challenge);
	bytesio.Write<lich::ChallengeInterface>(ChallengeInterface::Client);
	bytesio.Write<unsigned int>(1);
	bytesio.Write<ChallengeActionType>(ChallengeActionType::AbortChallenge);
	for (int i = 0; i < invited.size(); i++) {
		lich::GetSteamNetwork()->SendP2PPacket(invited[i].steamID, bytesio.Data(), bytesio.Size(), k_EP2PSendReliable, 2);
	}

	return true;
}

/*void lich::ChallengeHost::PlayersAcceptedCount(int* cnt)
{
	for (int i = 0; i < invited.size(); i++)
		if (invited[i].accepted)
			*cnt += 1;

	//return *cnt;
}*/

void lich::ChallengeHost::PlayerAcceptedCount(int* cnt)
{
	for (int i = 0; i < invited.size(); i++)
		if (invited[i].accepted == true)
			*cnt += 1;
}

void lich::ChallengeHost::HandleAcceptPacket(BytesIO* bytesio, CSteamID steamIDRemote)
{
	unsigned int strlen = bytesio->Read<unsigned int>();
	wchar_t buffer[16];
	for (int i = 0; i < strlen; i++) {
		buffer[i] = towlower(bytesio->Read<wchar_t>());
	}

	for (int i = 0; i < invited.size(); i++) {
		wchar_t buff[100];
		swprintf_s(buff, 100, L"[%s] [%s] \n", invited[i].name.c_str(), buffer);
		lich::DebugMessage(buff);
		if (!invited[i].name.compare(buffer)) {
			invited[i].accepted = true;
			invited[i].steamID = steamIDRemote;
		}
	}
}

void lich::ChallengeHost::HandleDeclinePacket(BytesIO* bytesio, CSteamID steamIDRemote)
{
	unsigned int strlen = bytesio->Read<unsigned int>();
	wchar_t buffer[16];
	for (int i = 0; i < strlen; i++) {
		buffer[i] = towlower(bytesio->Read<wchar_t>());
	}

	for (int i = 0; i < invited.size(); i++) {
		if (!invited[i].name.compare(buffer)) {
			invited[i].accepted = false;
			invited[i].steamID = steamIDRemote;
		}
	}
}

void lich::ChallengeHost::StartChallenge(cube::Creature* creature)
{
	BytesIO bytesio;
	bytesio.Write<lich::PacketID>(PacketID::Challenge);
	bytesio.Write<lich::ChallengeInterface>(ChallengeInterface::Client);
	bytesio.Write<unsigned int>(3);
	bytesio.Write<ChallengeActionType>(ChallengeActionType::PvPOn);
	bytesio.Write<ChallengeActionType>(ChallengeActionType::RecoverHP);
	bytesio.Write<ChallengeActionType>(ChallengeActionType::Teleport);
	bytesio.Write<long long>(posOne[0]);
	bytesio.Write<long long>(posOne[1]);
	bytesio.Write<long long>(posOne[2]);
	for (int i = 0; i < invited.size(); i++) {
		lich::GetSteamNetwork()->SendP2PPacket(invited[i].steamID, bytesio.Data(), bytesio.Size(), k_EP2PSendReliable, 2);
	}

	state = ChallengeState::InChallenge;
	actions.push_back(ChallengeAction(ChallengeActionType::PvPOn));
	actions.push_back(ChallengeAction(ChallengeActionType::RecoverHP));
	actions.push_back(ChallengeAction(ChallengeActionType::Teleport, LongVector3(posTwo[0], posTwo[1], posTwo[2])));
}
