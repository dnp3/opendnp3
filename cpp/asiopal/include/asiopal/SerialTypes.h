#ifndef __SERIAL_TYPES_H_
#define __SERIAL_TYPES_H_

#include <string>

namespace asiopal
{

/// Enumeration for setting serial port parity
enum ParityType {
	PAR_NONE = 0,
	PAR_EVEN = 1,
	PAR_ODD = 2
};

/// Enumeration for setting serial port flow control
enum FlowType {
	FLOW_NONE = 0,
	FLOW_HARDWARE = 1,
	FLOW_XONXOFF = 2
};

ParityType GetParityFromInt(int parity);
FlowType GetFlowTypeFromInt(int parity);

/// Settings structure for the serial port
struct SerialSettings {

	/// Defaults to the familiar 9600 8/N/1, no flow control
	SerialSettings() :
		mBaud(9600),
		mDataBits(8),
		mStopBits(1),
		mParity(PAR_NONE),
		mFlowType(FLOW_NONE)
	{}

	/// name of the port, i.e. "COM1" or "/dev/tty0"
	std::string mDevice;
	/// Baud rate of the port, i.e. 9600 or 57600
	int mBaud;
	/// Data bits, usually 8
	int mDataBits;
	/// Stop bits, usually set to 1
	int mStopBits;
	/// Parity setting for the port, usually PAR_NONE
	ParityType mParity;
	/// Flow control setting, usually FLOW_NONE
	FlowType mFlowType;
};

}

#endif

