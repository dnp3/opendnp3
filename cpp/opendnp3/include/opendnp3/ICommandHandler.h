#ifndef __I_COMMAND_HANDLER_H_
#define __I_COMMAND_HANDLER_H_

#include "ControlRelayOutputBlock.h"
#include "AnalogOutput.h"

namespace opendnp3
{

/**
* Interface used to dispatch SELECT / OPERATE / DIRECT OPERATE (Binary/Analog output) from the outstation to application code.
*/
class ICommandHandler
{
public:
	virtual ~ICommandHandler() {}

	/**
	* Select a ControlRelayOutputBlock - group 12 variation 1
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Select(const ControlRelayOutputBlock& arCommand, size_t aIndex) = 0;

	/**
	* Operate a ControlRelayOutputBlock - group 12 variation 1
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Operate(const ControlRelayOutputBlock& arCommand, size_t aIndex) = 0;

	/**
	* Direct operate a ControlRelayOutputBlock - group 12 variation 1
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex) = 0;

	/**
	* Select a 16 bit analog output - group 41 variation 2
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Select(const AnalogOutputInt16& arCommand, size_t aIndex) = 0;

	/**
	* Operate a 16 bit analog output - group 41 variation 2
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Operate(const AnalogOutputInt16& arCommand, size_t aIndex) = 0;

	/**
	* Direct operate a 16 bit analog output - group 41 variation 2
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex) = 0;

	/**
	* Select a 32 bit analog output - group 41 variation 1
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Select(const AnalogOutputInt32& arCommand, size_t aIndex) = 0;

	/**
	* Operate a 32 bit analog output - group 41 variation 1
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Operate(const AnalogOutputInt32& arCommand, size_t aIndex) = 0;

	/**
	* Direct operate a 32 bit analog output - group 41 variation 1
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex) = 0;

	/**
	* Select a single precision, floating point analog output - group 41 variation 3
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Select(const AnalogOutputFloat32& arCommand, size_t aIndex) = 0;

	/**
	* Operate a single precision, floating point analog output - group 41 variation 3
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Operate(const AnalogOutputFloat32& arCommand, size_t aIndex) = 0;

	/**
	* Direct operate a single precision, floating point analog output - group 41 variation 3
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex) = 0;

	/**
	* Select a double precision, floating point analog output - group 41 variation 4
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Select(const AnalogOutputDouble64& arCommand, size_t aIndex) = 0;

	/**
	* Operate a double precision, floating point analog output - group 41 variation 4
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus Operate(const AnalogOutputDouble64& arCommand, size_t aIndex) = 0;

	/**
	* Direct operate a double precision, floating point analog output - group 41 variation 4
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @return result of request
	*/
	virtual CommandStatus DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex) = 0;

};

}

#endif

