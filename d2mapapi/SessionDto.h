#pragma once
#include <string>
#include <json_dto/pub.hpp>

struct SessionDto
{
	SessionDto() = default;

	SessionDto(
		std::string id,
		unsigned int mapId,
		unsigned int difficulty)
		: m_id (std::move(id))
		, m_mapId{ mapId }
		, m_difficulty{ difficulty }
	{}

	template < typename JSON_IO >
	void
		json_io( JSON_IO& io )
	{
		io
			& json_dto::mandatory( "id", m_id )
			& json_dto::mandatory( "mapId", m_mapId )
			& json_dto::mandatory( "difficulty", m_difficulty );
	}

	std::string m_id;
	unsigned int m_mapId{};
	unsigned int m_difficulty{};
};

