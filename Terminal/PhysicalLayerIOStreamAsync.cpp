#include "PhysicalLayerIOStreamAsync.h"

#include <iostream>

using namespace boost::system;
using namespace std;

namespace apl
{

PhysicalLayerIOStreamAsync::PhysicalLayerIOStreamAsync(Logger* apLogger, boost::asio::io_service* apService) :
	PhysicalLayerSyncProxy(apLogger, apService)
{}

std::string PhysicalLayerIOStreamAsync::Read()
{
	string input;
	getline(cin, input);
	input.append("\r\n");
	return input;
}

void PhysicalLayerIOStreamAsync::Write(const std::string& arString)
{
	cout << arString;
	cout.flush();
}
}
