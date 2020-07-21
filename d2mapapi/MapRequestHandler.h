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

	restinio::request_handling_status_t get_map( const restinio::request_handle_t& req, const restinio::router::route_params_t& params) const;

};
