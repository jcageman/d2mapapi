#pragma once
#include <memory>
#include <restinio/all.hpp>
#include <restinio/router/express.hpp>


namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

namespace map
{
	std::unique_ptr<router_t > create_server_handler();
}
