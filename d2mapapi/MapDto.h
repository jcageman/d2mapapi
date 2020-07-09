#pragma once
#include "CollisionMap.h"

struct MapDto
{
	Point levelOrigin; // level top-left
	std::vector<std::vector<int>> map;
	std::map<std::string, AdjacentLevel> adjacentLevels;
	std::map<std::string, std::vector<Point>> npcs;
	std::map<std::string, std::vector<Point>> objects;

	template<typename Json_Io>
	void json_io( Json_Io& io ) {
		io	& json_dto::mandatory( "levelOrigin", levelOrigin )
			& json_dto::mandatory( "adjacentLevels", adjacentLevels )
			& json_dto::mandatory( "mapRows", map )
			& json_dto::mandatory( "npcs", npcs )
			& json_dto::mandatory( "objects", objects );
	}
};

namespace map
{
	MapDto MapToDto( CCollisionMap& map );
}

