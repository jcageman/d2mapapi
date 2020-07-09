#include "ServerHandler.h"

#include <memory>

#include "MapRequestHandler.h"

std::unique_ptr<router_t > map::create_server_handler()
{
	auto router = std::make_unique< router_t >();
	auto handler = std::make_shared< MapRequestHandler >();

	auto callHandler = [&]( auto method ) {
		using namespace std::placeholders;
		return std::bind( method, handler, _1, _2 );
	};

	// Handlers for '/sessions/' path.
	router->http_post("/sessions/", callHandler( &MapRequestHandler::post_session ) );

	router->http_delete("/sessions/:session_id", callHandler( &MapRequestHandler::delete_session ) );

	// Handler for '/sessions/:sessionid/areas/:areaid' path.
	router->http_get("/sessions/:session_id/areas/:area_id", callHandler( &MapRequestHandler::get_area ) );

	router->non_matched_request_handler(
		[]( auto req ) {
			return req->create_response( restinio::status_not_found() )
				.append_header_date_field()
				.connection_close()
				.done();
		} );

	return router;
}
