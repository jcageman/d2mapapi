#define _WIN32_WINNT 0x0A00
#include <iostream>
#include <string>

#include "D2Map.h"
#include "ServerHandler.h"



int main( int argc, const char* argv[] ) {
    int i;
	// Check command line arguments.
	if (argc < 2)
	{
		std::cerr << "Usage: program.exe {diablo2dir} [optionalArguments]\n";
		std::cerr << "\t-i or --ip\n\t\tspecify an IP address to use instead of localhost.\n\t\tex: -i 192.168.0.1 OR --ip=192.168.0.1\n";
		std::cerr << "\t-p or --port\n\t\tspecify an port address to use instead of the default 8080.\n\t\tex: -p 8080 OR --port=8080\n";
		std::cerr << "\t-h or --help\n\t\tprints this usage information\n";
		std::cerr << "Example: program.exe \"C:\\Diablo II1.13c\"\nor\nExample: program.exe \"C:\\Diablo II1.13c\" -i 0.0.0.0 -p 8888";
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
			std::regex rgx("--\\w+=(.+)");
			std::smatch match;
			std::string tmpArg = argv[i];

			if (tmpArg == "-h" || tmpArg == "--help")
			{
				std::cout << "Usage: program.exe {diablo2dir} [optionalArguments]\n";
				std::cout << "\t -i or --ip\n\t\tspecify an IP address to use instead of localhost.\n\t\tex: -i 192.168.0.1 OR --ip=192.168.0.1\n";
				std::cout << "\t-p or --port\n\t\tspecify an port address to use instead of the default 8080.\n\t\tex: -p 8080 OR --port=8080";
				return 0;
			}
			if (tmpArg == "-i") 
			{
				address = argv[i + 1];
			}
			if (tmpArg == "-p")
			{
				char *end;
				errno = 0;
				long val = strtol(argv[i + 1], &end, 10);
				if (errno || end == argv[i + 1] || *end != '\0' || val < 0 || val >= 0x10000) {
					std::cerr << "Failed to convert port argument to uint16 from string, ensure the value passed is a valid integer\n";
					return 1;
				}
				port = (uint16_t)val;
			}
			if (tmpArg.find("--ip") != std::string::npos)
			{	
				if (std::regex_search(tmpArg, match, rgx)) {
					address = match[1];
				} else {
					std::cerr << "Failed to find valid IP address in argument --ip, using localhost";
				}
			}
			if (tmpArg.find("--port") != std::string::npos) 
			{
				if (std::regex_search(tmpArg, match, rgx)) {
					port = std::stoi(match[1]);
				} else {
					std::cerr << "Failed to find valid port in argument --port, using 8080";
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