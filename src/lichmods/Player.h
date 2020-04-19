#pragma once

#include "lich.h"
#include <string>
#include <vector>

namespace lich {
	class Player 
	{
	public:
		std::wstring name;
		LongVector3 storedPosition;
		bool pvpState;
		cube::Creature* playerPointer;
		Player(): name(std::wstring()), storedPosition(LongVector3()), pvpState(false), playerPointer(NULL) {}

		void SetPlayerPointer(cube::Creature* ptr);
	};
}