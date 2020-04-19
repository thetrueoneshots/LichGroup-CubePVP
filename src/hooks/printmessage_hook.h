/* This file is not uses as a hook in the current version of the mod.
 * However it is kept here if you want an example on how to hook the
 * messages in the chat.
*/

extern "C" int ChatPrintHandler(std::wstring* msg) {
	cube::Creature* player = cube::GetGame()->GetPlayer();
	wchar_t* message = (wchar_t*)msg->c_str();

	wchar_t target_name[16];
	wchar_t sender_name[16];

	if (swscanf(message, L"/receivechallenge %s from %s", &target_name, &sender_name) == 2) {
		wchar_t received[100];
		wchar_t* t_name = (wchar_t*)toLowerCase(std::wstring(target_name)).c_str();
		wchar_t* s_name = (wchar_t*)toLowerCase(std::wstring(sender_name)).c_str();
		wchar_t* name = (wchar_t*)getLowerCaseName(player).c_str();
		if (!wcscmp(t_name, name)) {
			challengeRequest.receiveChallenge(std::wstring(s_name));
		}

		swprintf(received, L"I challenge %s [%s]!\n", &target_name, &sender_name);
		msg[0] = std::wstring(received);
	}
	else if (swscanf(message, L"/acceptedchallenge %s from %s", &target_name, &sender_name) == 2) {
		wchar_t received[100];
		wchar_t* t_name = (wchar_t*)toLowerCase(std::wstring(target_name)).c_str();
		wchar_t* s_name = (wchar_t*)toLowerCase(std::wstring(sender_name)).c_str();
		wchar_t* name = (wchar_t*)getLowerCaseName(player).c_str();

		if (!wcscmp(t_name, name)) {
			challengeRequest.challenge_accepted = true;
			challengeRequest.challenger_name = std::wstring(s_name);
		}

		swprintf(received, L"Accepted challenge from %s [%s]!\n", &target_name, &sender_name);
		msg[0] = std::wstring(received);
	}
	else if (swscanf(message, L"/abortedchallenge %s from %s", &target_name, &sender_name) == 2) {
		wchar_t received[100];
		wchar_t* t_name = (wchar_t*)toLowerCase(std::wstring(target_name)).c_str();
		wchar_t* s_name = (wchar_t*)toLowerCase(std::wstring(sender_name)).c_str();
		wchar_t* name = (wchar_t*)getLowerCaseName(player).c_str();

		if (!wcscmp(t_name, name)) {
			challengeRequest.challenge_aborted = true;
		}

		swprintf(received, L"Aborted challenge by %s [%s]!\n", &sender_name, &target_name);
		msg[0] = std::wstring(received);
	}
	return 0;
}

GETTER_VAR(void*, ASMChatHandler_jmpback);
GETTER_VAR(void*, ASMChatHandler_bail);
__attribute__((naked)) void ASMChatPrintHandler() {
	asm(".intel_syntax \n"
		"mov rax, rsp \n"
		"push rbp \n"
		"push rsi \n"
		"push rdi \n"
		"push r12 \n"
		"push r13 \n"
		"push r14 \n"
		"push r15 \n"
		"lea rbp, [rax-0x0E8] \n"

		PUSH_ALL
		"mov rcx, rdx \n"

		PREPARE_STACK

		"call ChatPrintHandler \n"

		RESTORE_STACK

		POP_ALL

		DEREF_JMP(ASMChatHandler_jmpback)
		".att_syntax \n"
		);
}

void SetupChatPrintHandler() {
	char* base = (char*)CWBase();
	WriteFarJMP(base + 0x26B670, (void*)&ASMChatPrintHandler);
	ASMChatHandler_jmpback = (void*)(base + 0x26B685);
	ASMChatHandler_bail = (void*)(base + 0x26BF0B);
}