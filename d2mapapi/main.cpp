#define _WIN32_WINNT 0x0A00
#include <iostream>
#include <string>

#include "D2Map.h"
#include "ServerHandler.h"



int main( int argc, const char* argv[] ) {

	// Check command line arguments.
	if (argc < 2)
	{
		std::cerr << "Usage: program.exe {diablo2dir} [addressOverride]\n";
		std::cerr << "Example: program.exe \"C:\\Diablo II1.13c\"\nor\nExample: program.exe \"C:\\Diablo II1.13c\" 0.0.0.0";
		return 1;
	}

	using namespace std::chrono;
	try
	{
		
		init( argv[1] );
		
		using traits_t =
			restinio::traits_t<
			restinio::asio_timer_manager_t,
			restinio::single_threaded_ostream_logger_t,
			router_t >;

		std::string address = "localhost";

		if (argc == 3) {
			address = argv[2];
		}

		restinio::run(
			restinio::on_this_thread< traits_t >()
			.address( address )
			.request_handler( map::create_server_handler() )
			.read_next_http_message_timelimit( 20s )
			.write_http_response_timelimit( 10s )
			.handle_request_timeout( 10s ) );
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}