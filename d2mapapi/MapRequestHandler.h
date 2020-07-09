#pragma once


#include <restinio/all.hpp>
#include <restinio/request_handler.hpp>
#include "Session.h"

namespace rr = restinio::router;

class MapRequestHandler
{
public:
	MapRequestHandler() = default;
	MapRequestHandler( const MapRequestHandler& ) = delete;
	MapRequestHandler( MapRequestHandler&& ) = delete;

	restinio::request_handling_status_t get_area(const restinio::request_handle_t& req, const restinio::router::route_params_t& params );

	restinio::request_handling_status_t post_session( const restinio::request_handle_t& req, const restinio::router::route_params_t& params);

	restinio::request_handling_status_t delete_session(const restinio::request_handle_t& req, const restinio::router::route_params_t& params );

};
