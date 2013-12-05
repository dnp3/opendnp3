#ifndef __DNP_HELPERS_H_
#define __DNP_HELPERS_H_

#include <string>


namespace opendnp3
{

class LinkFrame;

std::string RepairCRC(const std::string& arData);
bool IsFrameEqual(LinkFrame& frame, const std::string& arData);

}

#endif

