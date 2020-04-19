#include "Player.h"

void lich::Player::SetPlayerPointer(cube::Creature* ptr)
{
	playerPointer = ptr;
	char* str = ptr->entity_data.name;
	name = lich::toLowerCase(std::wstring(str, str + strlen(str)));
}
