#pragma once
#include <map>
#include <memory>
#include "CollisionMap.h"

class Session
{
public:
	Session( unsigned int mapid, unsigned int difficulty );
	~Session();

	CCollisionMap* GetMap(unsigned int areaid);
	unsigned int GetMapId() const;
	unsigned int GetDifficulty() const;
private:
	std::map<int, std::unique_ptr<CCollisionMap>> m_maps;
	unsigned int m_mapid;
	unsigned int m_difficulty;
	Act* pAct[5];
};

