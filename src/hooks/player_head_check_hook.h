extern "C" int CheckHeadSize(cube::Creature* player, cube::Creature* target) {
	if (player->entity_data.appearance.head_scale > 1.2f && target->entity_data.appearance.head_scale > 1.2f) {
		return 1;
	}
	return 0;
}

GETTER_VAR(void*, ASMPvpHandler_jmpback);
GETTER_VAR(void*, ASMPvpHandler_bail);
__attribute__((naked)) void ASMPvpHandler() {
	asm(".intel_syntax \n"
		
		PUSH_ALL
		PREPARE_STACK

		"call CheckHeadSize \n"

		RESTORE_STACK

		"test al, al \n"
		"jnz pvpon \n"

		POP_ALL

		"movzx r11d, byte ptr [rdx+0x60] \n"
		"cmp r11b, 07 \n"
		"jz bail \n"

		DEREF_JMP(ASMPvpHandler_jmpback)

		"pvpon: \n"
		POP_ALL
		"movzx r11d, byte ptr [rdx+0x60] \n"
		"cmp r11b, 00 \n"
		"jz bail \n"

		DEREF_JMP(ASMPvpHandler_jmpback)

		"bail: \n"
		DEREF_JMP(ASMPvpHandler_bail)

		".att_syntax \n"
		);
}

void SetupPlayerPvpHandler() {
	char* base = (char*)CWBase();
	WriteFarJMP(base + 0x50598, (void*)&ASMPvpHandler);
	ASMPvpHandler_jmpback = (void*)(base + 0x505A7);
	ASMPvpHandler_bail = (void*)(base + 0x5062C);
}