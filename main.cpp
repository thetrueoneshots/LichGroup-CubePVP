#include "main.h"

// Public variables
lich::ChallengeHandler challengeHandler;

// Includes for the self written hooks.
// #include "src/hooks/printmessage_hook.h" // Currently not included because of the steamnetworking. For usage, read the comment at the start of the file.
#include "src/hooks/player_head_check_hook.h"


/* Mod class containing all the functions for the mod.
*/
class Mod : GenericMod {
	bool initialised = false;

	// [Todo]:	Move these to the host class?
	//long long arena_coords[3] = { -304600797, 220564195, 6296698 };
	//long long arena_coords_2[3] = { -304614591, 212580979, 6296698 };

	/* Hook for the chat function. Triggers when a user sends something in the chat.
	 * @param	{std::wstring*} message
	 * @return	{int}
	*/
	virtual int OnChat(std::wstring* message) override {
		cube::Game* game = cube::GetGame();
		wchar_t* msg = (wchar_t*)message->c_str();
		wchar_t target_name[16];
		int index = 0;

		if (!wcscmp(msg, L"/p req") || !wcscmp(msg, L"/print requests")) {
			game->PrintMessage((wchar_t*)challengeHandler.PrintRequests().c_str());
			return 1;
		} 
		else if (!wcscmp(msg, L"/pvp")) {
			if (game->GetPlayer()->entity_data.appearance.head_scale < 1.1f) {
				game->GetPlayer()->entity_data.appearance.head_scale = 1.3f;
			}
			else {
				game->GetPlayer()->entity_data.appearance.head_scale = 1.0f;
			}
		}
		else if (!wcscmp(msg, L"/getinterface")) {
			lich::Challenge* ch = challengeHandler.GetChallenge();
			if (ch->GetInterfaceType() == lich::ChallengeInterface::Host)
				lich::PrintMessage(L"You are a host!");
			else
				lich::PrintMessage(L"You are a client!");
			return 1;
		}
		else if (!wcscmp(msg, L"/ch ac") || swscanf_s(msg, L"/ch ac %d", &index) == 1) {
			if (challengeHandler.AcceptRequest(index)) {
				game->PrintMessage(L"Accepted the challenge!\n");
			}
			else {
				game->PrintMessage(L"Could not accept challenge..\n");
			}
			return 1;
		}
		else if (!wcscmp(msg, L"/ch ab") || !wcscmp(msg, L"/challenge abort")) {
			challengeHandler.AbortChallenge();
			return 1;
		}
		else if (swscanf_s(msg, L"/challenge %ls", &target_name) == 1) { //Command to send a challenge to a person by name.
			// Check if you are not alone.
			// [Todo]: Let the challengehandler send the actual request after creating a challenge

			if (cube::SteamUser()->GetSteamID() != game->client.host_steam_id || game->host.connections.size() > 1) {

				cube::Creature* player = game->GetPlayer();
				std::wstring name = lich::getLowerCaseName(player).c_str();
				wchar_t* player_name = (wchar_t*)name.c_str();

				// Create and send the network packet
				BytesIO bytesio; //Packet object
				bytesio.Write<lich::PacketID>(lich::PacketID::ChallengeRequest); // Is it a request or challenge data.
				bytesio.Write<lich::ChallengeType>(lich::ChallengeType::OneVersusOne); //Packet ID
				bytesio.Write<unsigned int>(wcslen(target_name)); //Target name length
				bytesio.Write<unsigned int>(wcslen(player_name)); //Player name length

				std::vector<std::wstring> vec;
				vec.push_back(std::wstring(target_name));
				challengeHandler.CreateChallenge(lich::ChallengeType::OneVersusOne, vec);

				for (int i = 0; i < wcslen(target_name); i++) {
					bytesio.Write<wchar_t>(target_name[i]); //Target name
				}

				for (int i = 0; i < wcslen(player_name); i++) {
					bytesio.Write<wchar_t>(player_name[i]); //Target name
				}

				// [Todo]:	Send to all people in the chat
				if (cube::SteamUser()->GetSteamID() != game->client.host_steam_id) {
					cube::SteamNetworking()->SendP2PPacket(game->client.host_steam_id, bytesio.Data(), bytesio.Size(), k_EP2PSendReliable, 2);
				}
				for (auto const& conn : game->host.connections) {
					cube::SteamNetworking()->SendP2PPacket(conn.first, bytesio.Data(), bytesio.Size(), k_EP2PSendReliable, 2);
				}
				//cube::SteamNetworking()->SendP2PPacket(game->client.host_steam_id, bytesio.Data(), bytesio.Size(), k_EP2PSendReliable, 2);

				// [Todo]:	Add name of the person that is challenged

				game->PrintMessage(L"You challenged someone!\n");

			}

			return 1;
		}

		return 0;
	}

	/* Function hook that gets called every game tick.
	 * @param	{cube::Game*} game
	 * @return	{void}
	*/
	virtual void OnGameTick(cube::Game* game) override {
		// Intialising the variables that need pointers not yet initialised in the initialise function.
		if (!initialised) {
			initialised = true;
			challengeHandler.SetPlayerPointer(game->GetPlayer());
		}

		ISteamNetworking* network = cube::SteamNetworking();
		uint32 msgSize = 0;
		if (network->IsP2PPacketAvailable(&msgSize, 2)) 
		{
			void* packet = std::malloc(msgSize);
			CSteamID steamIDRemote;
			uint32 bytesRead = 0;
			if (network->ReadP2PPacket(packet, msgSize, &bytesRead, &steamIDRemote, 2))
			{
				BytesIO bytesio((unsigned char*)packet, bytesRead);
				challengeHandler.HandlePacket(&bytesio, steamIDRemote);
			}
			free(packet);
		}

		challengeHandler.OnGameTick();
	}

	/* Function hook that gets called on intialization of cubeworld.
	 * [Note]:	cube::GetGame() is not yet available here!!
	 * @return	{void}
	*/
	virtual void Initialize() override {
		// SetupChatPrintHandler(); Currently not used because of the improved code using steamnetworking.
		SetupPlayerPvpHandler();
	}
};

// Export of the mod created in this file, so that the modloader can see and use it.
EXPORT Mod* MakeMod() {
	return new Mod();
}
/*
Challenge opponent [gold [star [class [specialization]]]] / Make a GUI
Maybe bet artifacts/pets/EVERYTHING???

1. [v] Teleport both players to arena.
2. [x] Subtract gold (check if enough!) and set items/classes.
3. [x] Lock controls for 3 seconds (countdown in chat) then start.
4. [v] PvP mode.
5. [x] Give rewards.
6. [x] Restore items/classes.
7. [v] Restore locations. 

*/