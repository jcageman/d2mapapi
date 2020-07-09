#pragma once
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <codecvt>
#include <locale>

using HostInetInfo = boost::asio::ip::tcp::resolver::iterator;

class NetworkUtils {
private:
	static HostInetInfo queryHostInetInfo();
	static std::wstring hostIP( unsigned short family );
public:
	// gets the host IP4 string formatted
	static std::wstring hostIP4() {
		return hostIP( AF_INET );
	}

	// gets the host IP6 string formatted
	static std::wstring hostIP6() {

		return hostIP( AF_INET6 );
	}
	static std::wstring hostName() {
		return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(boost::asio::ip::host_name());
	}
};

