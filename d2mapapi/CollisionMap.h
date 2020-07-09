#pragma once

#include <map>
#include <string>
#include <json_dto/pub.hpp>
#include "D2Structs.h"

struct Point
{
	int x{};
	int y{};

	template < typename JSON_IO >
	void
		json_io( JSON_IO& io )
	{
		io
			& json_dto::mandatory( "x", x )
			& json_dto::mandatory( "y", y );
	}
};

struct AdjacentLevel
{
	std::vector<Point> exits{};
	Point levelOrigin{};
	int width{};
	int height{};

	template < typename JSON_IO >
	void
		json_io( JSON_IO& io )
	{
		io
			& json_dto::mandatory( "exits", exits )
			& json_dto::mandatory( "origin", levelOrigin )
			& json_dto::mandatory( "width", width )
			& json_dto::mandatory( "height", height );
	}
};

class CCollisionMap
{
private:	
	unsigned int areaid;
	Act* pAct;
public:
	CCollisionMap(Act* pAct, unsigned int areaid);
	void build();

	Point m_levelOrigin; // level top-left
	std::vector<std::vector<int>> m_map;
	std::map<std::string, AdjacentLevel> m_adjacentLevels;
	std::map<std::string, std::vector<Point>> m_npcs;
	std::map<std::string, std::vector<Point>> m_objects;

};
