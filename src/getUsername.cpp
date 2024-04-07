#define WIN32_LEAN_AND_MEAN
#include "functions.hpp"
#include <Windows.h>
#include <boost/asio/ip/host_name.hpp>

void Functions::getSystemUsername()
{
	const auto host_name = boost::asio::ip::host_name();
	std::cout << "System Username: " << host_name << "\n";
}