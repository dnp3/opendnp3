#ifndef __COMMAND_TASK_H_
#define __COMMAND_TASK_H_

#include "MasterTaskBase.h"

#include <opendnp3/APDUConstants.h>
#include <opendnp3/CommandResponse.h>

#include <openpal/Visibility.h>
#include <openpal/Logger.h>

#include "gen/FunctionCode.h"

#include <functional>
#include <queue>

namespace opendnp3
{

// Base class with machinery for performing command operations
class DLL_LOCAL CommandTask : public MasterTaskBase
{
	typedef std::function<CommandStatus (const APDU&)> Validator;
	typedef std::function<Validator (APDU&, FunctionCode)> Formatter;
	typedef std::function<void (CommandResponse)> Responder;

public:
	CommandTask(openpal::Logger);

	void Configure(const Formatter& arFormatter, const Responder& arResponder);
	void AddCommandCode(FunctionCode aCode);

	void ConfigureRequest(APDU& arAPDU);

	std::string Name() const;

protected:

	Formatter mFormatter;
	Validator mValidator;
	Responder mResponder;

	// override from base class
	void OnFailure();

private:

	std::deque<FunctionCode> mCodes;

	void Respond(CommandStatus aStatus);

	TaskResult _OnPartialResponse(const APDU&);
	TaskResult _OnFinalResponse(const APDU&);
};

} //ens ns

#endif
