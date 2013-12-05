#ifndef _ASIO_SERIAL_HELPERS_H_
#define _ASIO_SERIAL_HELPERS_H_

#include <boost/asio.hpp>

#include <openpal/Visibility.h>

#include "SerialTypes.h"

namespace boost
{
namespace asio
{
typedef basic_serial_port<> serial_port;
}
}

namespace asiopal
{

// Serial port configuration functions "free" to keep the classes simple.
void DLL_LOCAL Configure(SerialSettings& arSettings, boost::asio::serial_port& arPort, boost::system::error_code& ec);
void DLL_LOCAL Configure(SerialSettings& arSettings, boost::asio::serial_port& arPort);

}

#endif
