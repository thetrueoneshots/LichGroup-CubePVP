cmake_minimum_required (VERSION 3.8)
project(project_LichModsPvP)
add_subdirectory(cwmods)
add_library (LichModsPvP SHARED "main.cpp" 
	"src/lichmods/lich.cpp" 
	"src/lichmods/ChallengeHandler.cpp" 
	"src/lichmods/ChallengeRequestList.cpp"
	"src/lichmods/ChallengeRequest.cpp"
	"src/lichmods/Challenge.cpp"
	"src/lichmods/ChallengeClient.cpp"
	"src/lichmods/Player.cpp"
 "src/lichmods/ChallengeAction.cpp"
 "src/lichmods/ChallengeHost.cpp" "src/lichmods/ChallengeRule.cpp")
target_link_libraries (LichModsPvP LINK_PUBLIC CWSDK)