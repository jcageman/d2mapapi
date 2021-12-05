#pragma once
#include "D2Structs.h"
#include <cstdint>

#define API_EXPORT __declspec(dllexport)
extern "C" {
	API_EXPORT bool Initialize(const wchar_t* path);
	API_EXPORT Level* GetLevel(ActMisc* misc, uint32_t levelno);
	API_EXPORT void InitLevel(Level* pLevel);
	API_EXPORT void AddRoomData(Act* pAct, int32_t levelid, int32_t xpos, int32_t ypos, Room1* pRoom);
	API_EXPORT void RemoveRoomData(Act* pAct, int32_t levelid, int32_t xpos, int32_t ypos, Room1* pRoom);
	API_EXPORT Act* LoadAct(uint32_t actno, uint32_t seed, uint32_t difficulty, uint32_t TownLevelId);
	API_EXPORT void UnloadAct(Act* pAct);
}