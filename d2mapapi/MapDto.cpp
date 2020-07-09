#include "MapDto.h"

MapDto map::MapToDto(CCollisionMap& map)
{
	auto mapDto = MapDto{};
	mapDto.adjacentLevels = map.m_adjacentLevels;
	mapDto.map = map.m_map;
	mapDto.levelOrigin = map.m_levelOrigin;
	mapDto.npcs = map.m_npcs;
	mapDto.objects = map.m_objects;

	return mapDto;
}
