#include "ChallengeRule.h"

boolean lich::ChallengeRule::ExecuteRule(cube::Creature* creature, std::vector<LongVector3>* positions)
{
	switch (type)
	{
	case ChallengeRuleType::OnDeath:
		if (creature->entity_data.HP <= 0) {
			if (creature->entity_data.classType != 5 || creature->entity_data.time_since_ability < 30000.0f) {
				// [Todo]:	Handle on death trigger
				lich::PrintMessage(L"You died! \n");
				return true;
			}
		}
		return false;
		break;
	default:
		return true;
		break;
	}
}