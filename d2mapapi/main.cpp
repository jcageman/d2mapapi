#define _WIN32_WINNT 0x0A00
#include <iostream>
#include <string>

#include "D2Map.h"
#include "ServerHandler.h"
#include "Helpers.h"

int main( int argc, const char* argv[] ) {
    int i;

	// Check command line arguments.
	if (argc < 2)
	{
		printHelp();
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
		std::uint16_t port = 8080;

		// handle args in this loop:
		for(i=2; i<argc; ++i)
		{	
			std::string tmpArg = argv[i];
			std::string argVal;

			if (tmpArg == "-h" || tmpArg == "--help")
			{
				printHelp();
				return 0;
			}

			if (tmpArg == "-i") 
				address = argv[i + 1];

			if (tmpArg == "-p")
			{
				std::string tmpPort = argv[i + 1];
				port = std::stoi(tmpPort);
			}

			if (tmpArg.find("--ip") != std::string::npos)
			{
				argVal = extractArg(tmpArg);
				if (argVal == "") {
					return 1;
				}
				else {
					address = argVal;
				}
			}


			if (tmpArg.find("--port") != std::string::npos)
			{
				argVal = extractArg(tmpArg);
				if (argVal == "") {
					return 1;
				}
				else {
					port = std::stoi(extractArg(tmpArg));
				}
			}
		}

		restinio::run(
			restinio::on_this_thread< traits_t >()
			.address( address )
			.port( port )
			.request_handler( map::create_server_handler() )
			.read_next_http_message_timelimit( 10s )
			.write_http_response_timelimit( 1s )
			.handle_request_timeout( 1s ) );
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
