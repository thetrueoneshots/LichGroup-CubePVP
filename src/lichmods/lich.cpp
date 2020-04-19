#include "lich.h"

/* 
* Function to convert a float coordinate to cubeworld dots.
* @param	{float} toConvert
* @return	{long long}
*/
long long convertCoord(float coord) {
	return 0x2000000 + ((long long)(coord * cube::BLOCKS_PER_MAP_CHUNK * cube::DOTS_PER_BLOCK));
}


void lich::PrintMessage(wchar_t* text)
{
	cube::GetGame()->PrintMessage(text);
}

void lich::DebugMessage(wchar_t* text)
{
	if (DEBUG_MODE)
		PrintMessage(text);
}

ISteamNetworking* lich::GetSteamNetwork()
{
	return cube::SteamNetworking();
}

/* Function to get te lower case name of a creature.
 * @param	{cube::Creature*} creature
 * @return	{std::wstring}
*/
std::wstring lich::getLowerCaseName(cube::Creature* creature) {
	char* str = creature->entity_data.name;
	std::wstring wstr = std::wstring(str, str + strlen(str));
	std::transform(wstr.begin(), wstr.end(), wstr.begin(),
		[](unsigned char c) { return std::tolower(c); });
	wstr.shrink_to_fit();
	return wstr;
}

/* Function to get te lower case of a wstring.
 * @param	{std::wstring} creature
 * @return	{std::wstring}
*/
std::wstring lich::toLowerCase(std::wstring string) {
	std::transform(string.begin(), string.end(), string.begin(),
		[](unsigned char c) { return std::tolower(c); });
	string.shrink_to_fit();
	return string;
}

/*
* Function to teleport a creature to a target position. Position is defined
* as LongVector3.
* [Note]:	The longvector 3 contains the position in dots, visible in CheatEngine using Geo's cheattable.
*			This is also the internal cubeworld representation.
* @param	{cube::Creature*} creature
* @param	{LongVector3} target
* @return	{void}
*/
void lich::teleport(cube::Creature* creature, LongVector3 target) {
	LongVector3* position = &creature->entity_data.position;
	position->x = target.x;
	position->y = target.y;
	position->z = target.z;
}

/* 
* Function to teleport a creature to a target position. Position is as float coordinates,
* which will be converted before doing teleporting the player. 
* [Note]:	Float coordinates is what cubeworld shows you on the screen.
* @param	{cube::Creature*} creature
* @param	{float[]} target
* @return	{void}
*/
void lich::teleport(cube::Creature* creature, float target[3]) {
	LongVector3* position = &creature->entity_data.position;
	position->x = convertCoord(target[0]);
	position->y = convertCoord(target[1]);
	position->z = convertCoord(target[2]);
}

/* 
* Function to teleport a creature to a target position. Position is as dots,
* which will be converted before doing teleporting the player.
* [Note]:	Long long coordinates is what CheatEngine shows you as position. Internal cubeworld representation.
* @param	{cube::Creature*} creature
* @param	{long long[]} target
* @return	{void}
*/
void lich::teleport(cube::Creature* creature, long long target[3]) {
	LongVector3* position = &creature->entity_data.position;
	position->x = target[0];
	position->y = target[1];
	position->z = target[2];
}
