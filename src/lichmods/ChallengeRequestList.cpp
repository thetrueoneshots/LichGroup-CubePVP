#include "ChallengeRequestList.h"

lich::ChallengeRequest* lich::ChallengeRequestList::AcceptRequest(int i, std::wstring name)
{
	if (i < 0 || i >= challengeRequests.size())
		return NULL;
	challengeRequests[i].AcceptRequest(name);
	return &(challengeRequests[i]);
}

bool lich::ChallengeRequestList::DeclineRequest(int i)
{
	int size = challengeRequests.size();
	if (i < 0 || i >= size)
		return false;
	challengeRequests[i].DeclineRequest();
	challengeRequests.erase(challengeRequests.begin() + i);
	return challengeRequests.size() == size - 1;
}

bool lich::ChallengeRequestList::DeclineAllRequests()
{
	for (int i = 0; i < challengeRequests.size(); i++) {
		challengeRequests[i].DeclineRequest();
	}
	challengeRequests.clear();
	return challengeRequests.size() == 0;
}

std::wstring lich::ChallengeRequestList::PrintRequests()
{
	int size = challengeRequests.size();
	std::wstring str = std::wstring(L"RequestList: \n");
	for (int i = 0; i < size; i++) {
		std::wstring str2 = challengeRequests[i].PrintRequest();
		wchar_t buffer[100];
		swprintf(buffer, 100, L"Request %d:\n", i);
		str.insert(str.size(), std::wstring(buffer));
		str.insert(str.size(), str2);
	}
	return str;
}

int lich::ChallengeRequestList::GetRequestCount()
{
	return challengeRequests.size();
}



// [Todo]:	Actually read the package and put a challengeType and name in the request
bool lich::ChallengeRequestList::HandlePacket(BytesIO* bytesio, CSteamID source, cube::Creature* ptr)
{
	lich::ChallengeType challengeType = bytesio->Read<lich::ChallengeType>();

	// Read the sizes of the name from the packet
	const unsigned int target_name_size = bytesio->Read<unsigned int>();
	const unsigned int challenger_name_size = bytesio->Read<unsigned int>();

	// Create buffers of the size of the name (max size is 16)
	wchar_t target_name[16];
	wchar_t challenger_name[16];

	// Read the strings
	for (int i = 0; i < target_name_size; i++) {
		target_name[i] = (wchar_t)bytesio->Read<wchar_t>();
	}
	target_name[target_name_size] = 0;
	for (int i = 0; i < challenger_name_size; i++) {
		challenger_name[i] = (wchar_t)bytesio->Read<wchar_t>();
	}
	challenger_name[challenger_name_size] = 0;

	std::wstring name = lich::getLowerCaseName(ptr);
	name.shrink_to_fit();

	// Check for the correct player
	if (!wcscmp(target_name, name.c_str())) {
		// Set that there is a challenge
		int size = challengeRequests.size();
		challengeRequests.push_back(lich::ChallengeRequest(challengeType, source, std::wstring(challenger_name)));


		// Print that there is a challenge
		wchar_t buffer[100];
		swprintf(buffer, 100, L"Player %ls[%d] challenged player %ls[%d]!\n", &challenger_name, challenger_name_size, &target_name, target_name_size);
		lich::PrintMessage(buffer);

		return challengeRequests.size() == size + 1;
	}

	return false;
}
