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
	router->http_get("/maprequests", callHandler( &MapRequestHandler::get_map ) );

	router->non_matched_request_handler(
		[]( auto req ) {
			return req->create_response( restinio::status_not_found() )
				.append_header_date_field()
				.connection_close()
				.done();
		} );

	return router;
}
