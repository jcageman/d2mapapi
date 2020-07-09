#include "CollisionMap.h"
#include <iostream>
#include <vector>
#include <map>
#include "D2Map.h"
#include "D2Ptrs.h"

constexpr int unit_type_npc = 1;
constexpr int unit_type_object = 2;
constexpr int unit_type_tile = 5;

CCollisionMap::CCollisionMap(Act* pAct, unsigned int areaid)
{
	this->pAct = pAct;
	this->areaid = areaid;
}

void CCollisionMap::build()
{

	Level* pLevel = GetLevel(pAct->pMisc, areaid);

	if (pLevel)
	{
		if (!pLevel->pRoom2First)
		{
			D2COMMON_InitLevel(pLevel);
		}

		if (pLevel->pRoom2First)
		{
			m_levelOrigin.x = pLevel->dwPosX * 5;
			m_levelOrigin.y = pLevel->dwPosY * 5;

			const int width = pLevel->dwSizeX * 5;
			const int height = pLevel->dwSizeY * 5;
			m_map = std::vector<std::vector<int>>(height, std::vector<int>( width, -1) );

			for (Room2* pRoom2 = pLevel->pRoom2First; pRoom2; pRoom2 = pRoom2->pRoom2Next)
			{
				BOOL bAdded = FALSE;

				if (!pRoom2->pRoom1)
				{
					bAdded = TRUE;
					D2COMMON_AddRoomData( pAct, pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, NULL );
				}

				// levels near
				for (DWORD i = 0; i < pRoom2->dwRoomsNear; i++)
				{
					if (pLevel->dwLevelNo != pRoom2->pRoom2Near[i]->pLevel->dwLevelNo)
					{
						const auto originX = static_cast<int>(pRoom2->pRoom2Near[i]->pLevel->dwPosX * 5);
						const auto originY = static_cast<int>(pRoom2->pRoom2Near[i]->pLevel->dwPosY * 5);
						const auto origin = Point{ originX , originY };
						const auto newLevelWidth = pRoom2->pRoom2Near[i]->pLevel->dwSizeX * 5;
						const auto newLevelHeight = pRoom2->pRoom2Near[i]->pLevel->dwSizeY * 5;

						auto levelNumber = std::to_string(pRoom2->pRoom2Near[i]->pLevel->dwLevelNo);
						auto& adjacentLevel = m_adjacentLevels[levelNumber];
						adjacentLevel.levelOrigin = origin;
						adjacentLevel.width = newLevelWidth;
						adjacentLevel.height = newLevelHeight;
					}
				}

				// add collision data
				if (pRoom2->pRoom1 && pRoom2->pRoom1->Coll)
				{
					const int x = pRoom2->pRoom1->Coll->dwPosGameX - m_levelOrigin.x;
					const int y = pRoom2->pRoom1->Coll->dwPosGameY - m_levelOrigin.y;
					const int cx = pRoom2->pRoom1->Coll->dwSizeGameX;
					const int cy = pRoom2->pRoom1->Coll->dwSizeGameY;
					const int nLimitX = x + cx;
					const int nLimitY = y + cy;

					WORD* p = pRoom2->pRoom1->Coll->pMapStart;
					for (int j = y; j < nLimitY; j++)
					{
						for (int i = x; i < nLimitX; i++)
						{
							m_map[j][i] = *p++;
						}
					}
				}

				// add unit data
				for (PresetUnit* pPresetUnit = pRoom2->pPreset; pPresetUnit; pPresetUnit = pPresetUnit->pPresetNext)
				{
					// npcs
					if (pPresetUnit->dwType == unit_type_npc)
					{
						const auto npcX = static_cast<int>(pRoom2->dwPosX * 5 + pPresetUnit->dwPosX);
						const auto npcY = static_cast<int>(pRoom2->dwPosY * 5 + pPresetUnit->dwPosY);
						auto fileNumber = std::to_string( pPresetUnit->dwTxtFileNo );
						m_npcs[fileNumber].push_back( Point{npcX, npcY});
					}

					// objects
					if (pPresetUnit->dwType == unit_type_object)
					{
						const auto objectX = static_cast<int>(pRoom2->dwPosX * 5 + pPresetUnit->dwPosX);
						const auto objectY = static_cast<int>(pRoom2->dwPosY * 5 + pPresetUnit->dwPosY);
						auto fileNumber = std::to_string( pPresetUnit->dwTxtFileNo );
						m_objects[fileNumber].push_back( Point{ objectX, objectY } );
					}

					// level exits
					if (pPresetUnit->dwType == unit_type_tile)
					{
						for (RoomTile* pRoomTile = pRoom2->pRoomTiles; pRoomTile; pRoomTile = pRoomTile->pNext)
						{
							if (*pRoomTile->nNum == pPresetUnit->dwTxtFileNo)
							{
								const auto exitX = static_cast<int>(pRoom2->dwPosX * 5 + pPresetUnit->dwPosX);
								const auto exitY = static_cast<int>(pRoom2->dwPosY * 5 + pPresetUnit->dwPosY);

								auto levelNumber = std::to_string( pRoomTile->pRoom2->pLevel->dwLevelNo );
								m_adjacentLevels[levelNumber].exits.push_back(Point{ exitX , exitY });
							}
						}
					}
				}

				if (bAdded)
				{
					D2COMMON_RemoveRoomData( pAct, pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, NULL );
				}
			}
		}
	}
}