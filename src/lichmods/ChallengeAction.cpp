#include "ChallengeAction.h"

void lich::ChallengeAction::ExecuteAction(cube::Creature* creature, std::vector<LongVector3>* positions)
{
	switch (type)
	{
	case ChallengeActionType::Teleport:
		positions->push_back(creature->entity_data.position);
		lich::teleport(creature, position);
		break;
	case ChallengeActionType::TeleportBack:
		if (positions->empty())
			return;
		lich::teleport(creature, positions->back());
		positions->pop_back();
		break;
	case ChallengeActionType::PvPOff:
		creature->entity_data.appearance.head_scale = 1.0f;
		break;
	case ChallengeActionType::PvPOn:
		creature->entity_data.appearance.head_scale = 1.3f;
		break;
	case ChallengeActionType::RecoverHP:
		creature->entity_data.HP = creature->GetMaxHP();
		break;
	default:
		break;
	}
}