#pragma once

struct NewSessionDto
{
	NewSessionDto() = default;

	NewSessionDto(
		unsigned int difficulty,
		unsigned int mapId )
		: m_difficulty{ std::move( difficulty ) }
		, m_mapId{ std::move( mapId ) }
	{}

	template < typename JSON_IO >
	void
		json_io( JSON_IO& io )
	{
		io
			& json_dto::mandatory( "mapid", m_mapId )
			& json_dto::mandatory( "difficulty", m_difficulty );
	}

	unsigned int m_mapId;
	unsigned int m_difficulty;
};

