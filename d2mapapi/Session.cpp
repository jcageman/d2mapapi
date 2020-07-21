#include "Session.h"

#include <iostream>

#include "D2Ptrs.h"

static const unsigned int ActLevels[] = { 1, 40, 75, 103, 109, 137 };

namespace Helpers
{
	unsigned int getAct( unsigned int areaid )
	{
		for (int i = 1; i < 5; i++)
		{
			if (areaid < ActLevels[i])
			{
				return i - 1;
			}
		}

		return 4;
	}
}

Session::Session(unsigned mapid, unsigned difficulty)
{
	m_mapid = mapid;
	m_difficulty = difficulty;

	for (int i = 0; i < 5; i++)
	{
		pAct[i] = D2COMMON_LoadAct( i, mapid, TRUE, FALSE, difficulty, NULL, ActLevels[i], D2CLIENT_LoadAct_1, D2CLIENT_LoadAct_2 );
	}
}

Session::~Session()
{
	for (int i = 0; i < 5; i++)
	{
		D2COMMON_UnloadAct( pAct[i] );
	}

}

CCollisionMap* Session::GetMap(unsigned int areaid)
{
	if (m_maps.find( areaid ) == m_maps.end())
	{
		auto map = std::make_unique<CCollisionMap>(pAct[Helpers::getAct( areaid )], areaid );
		map->build();
		m_maps[areaid] = std::move(map);
	}

	return m_maps[areaid].get();
}

unsigned Session::GetMapId() const
{
	return m_mapid;
}

unsigned Session::GetDifficulty() const
{
	return m_difficulty;
}
