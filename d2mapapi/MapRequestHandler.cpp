#include "MapRequestHandler.h"

#include <mutex>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <json_dto/pub.hpp>
#include "MapDto.h"
#include "SessionDto.h"
#include "NewSessionDto.h"

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

static boost::uuids::random_generator  m_session_id_generator;
static std::map<std::string, std::unique_ptr<Session>> m_sessions;
static std::mutex m;

restinio::request_handling_status_t MapRequestHandler::get_area(const restinio::request_handle_t& req, const restinio::router::route_params_t& params)
{
	std::scoped_lock lock( m );
	auto resp = init_resp( req->create_response() );
	const auto session_id = restinio::cast_to<std::string>( params["session_id"] );
	const auto area_id = restinio::cast_to<unsigned int>( params["area_id"] );
	const auto session = m_sessions.find( session_id );
	if (session != m_sessions.end())
	{
		const auto map = session->second->GetMap( area_id );

		resp.set_body( json_dto::to_json( map::MapToDto(*map) ) );
	}
	else
	{
		resp.header().status_line( restinio::status_not_found() );
	}

	return resp.done();
}

restinio::request_handling_status_t MapRequestHandler::post_session(const restinio::request_handle_t& req, const restinio::router::route_params_t& params)
{
	std::scoped_lock lock( m );
	auto resp = init_resp( req->create_response() );

	try
	{
		auto newSession = json_dto::from_json< NewSessionDto >( req->body() );
		std::string uuid = boost::uuids::to_string( m_session_id_generator());
		m_sessions[uuid] = std::make_unique<Session>( newSession.m_mapId, newSession.m_difficulty );

		const auto createdSession = SessionDto( uuid, newSession.m_difficulty, newSession.m_mapId );
		resp.set_body( json_dto::to_json( createdSession ) );
		resp.header().status_line( restinio::status_created() );
	}
	catch (const std::exception&)
	{
		resp.header().status_line( restinio::status_bad_request() );
	}

	return resp.done();
}

restinio::request_handling_status_t MapRequestHandler::delete_session(const restinio::request_handle_t& req, const restinio::router::route_params_t& params )
{
	std::scoped_lock lock( m );
	auto resp = init_resp( req->create_response() );
	resp.header().status_line( restinio::status_no_content() );
	const auto session_id = restinio::cast_to<std::string>( params["session_id"] );
	const auto session = m_sessions.find( session_id );
	if (session != m_sessions.end())
	{
		m_sessions.erase( session );
	}

	return resp.done();
}
