#include "MapRequestHandler.h"

#include <mutex>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <json_dto/pub.hpp>
#include "MapDto.h"

template < typename RESP >
static RESP
init_resp( RESP resp )
{
	resp
		.append_header( "Server", "Diablo 2 map server /v.0.1" )
		.append_header_date_field()
		.append_header( "Content-Type", "text/plain; charset=utf-8" );

	return resp;
}

static std::unique_ptr<Session> m_session;
static std::mutex m;

restinio::request_handling_status_t MapRequestHandler::get_map(const restinio::request_handle_t& req, const restinio::router::route_params_t& params) const
{
	std::scoped_lock lock( m );
	auto resp = init_resp( req->create_response() );

	try
	{
		const auto queryparameters = restinio::parse_query( req->header().query() );
		const auto mapId = restinio::cast_to<unsigned int>( queryparameters[ "mapid" ] );
		const auto difficulty = restinio::cast_to<unsigned int>( queryparameters[ "difficulty" ] );
		const auto areaId = restinio::cast_to<unsigned int>( queryparameters[ "areaid" ] );
		if(!m_session || m_session->GetDifficulty() != difficulty || m_session->GetMapId() != mapId )
		{
			m_session.reset(); // first ensure we unload data
			m_session = std::make_unique<Session>( mapId, difficulty );
		}

		CCollisionMap* const map = m_session->GetMap( areaId );

		resp.set_body( json_dto::to_json( map::MapToDto( *map ) ) );
	}
	catch (const std::exception&)
	{
		resp.header().status_line( restinio::status_bad_request() );
	}

	return resp.done();
}