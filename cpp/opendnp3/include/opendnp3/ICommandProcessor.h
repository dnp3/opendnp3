#ifndef __I_COMMAND_PROCESSOR_H_
#define __I_COMMAND_PROCESSOR_H_

#include "AnalogOutput.h"
#include "ControlRelayOutputBlock.h"
#include "CommandResponse.h"

#include <functional>

namespace opendnp3
{

/**
* Interface used to dispatch SELECT / OPERATE / DIRECT OPERATE from application code to a master
*/
class ICommandProcessor
{
public:

	/**
	* Select and operate a ControlRelayOutputBlock
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void SelectAndOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) = 0;

	/**
	* Direct operate a ControlRelayOutputBlock
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) = 0;

	/**
	* Select and operate a 16 bit analog output
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void SelectAndOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) = 0;

	/**
	* Direct operate a 16 bit analog output
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) = 0;

	/**
	* Select and operate a 32 bit analog output
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void SelectAndOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) = 0;

	/**
	* Direct operate a 32 bit analog output
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) = 0;

	/**
	* Select and operate a single precision analog output
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void SelectAndOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) = 0;

	/**
	* Direct operate a single precision analog output
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) = 0;

	/**
	* Select and operate a double precision analog output
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void SelectAndOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) = 0;

	/**
	* Direct operate a double precision analog output
	* @param arCommand command to operate
	* @param aIndex index of the command
	* @param aCallback callback that will be invoked upon completion or failure
	*/
	virtual void DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) = 0;
};

}

#endif
