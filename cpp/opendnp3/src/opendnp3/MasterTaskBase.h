#ifndef __MASTER_TASK_BASE_H_
#define __MASTER_TASK_BASE_H_

#include <string>

#include <openpal/Visibility.h>
#include <openpal/Loggable.h>


namespace opendnp3
{

class ITask;
class APDU;
class IINField;

enum TaskResult {
	TR_FAIL,		// The task fails, further responses are ignored

	TR_SUCCESS,		// The tasks is successful and complete

	TR_CONTINUE,    // The task is not yet complete. If OnFinalResponse
	// returns CONTINUE, it's a multi request task
};

/**
 * A generic interface for defining master request/response style tasks.
 */
class DLL_LOCAL MasterTaskBase : public openpal::Loggable
{
public:

	MasterTaskBase(openpal::Logger& arLogger);

	/**
	 * Sets the task completion handler and calls the overiddable _Init()
	 * function.
	 */
	virtual void Init() {}

	/**
	 * Configure a request APDU.  A container APDU instance is passed in
	 * as the argument, and the implementing function should setup the
	 * APDU instance as is appropriate for the subclass implementing the
	 * behavior.
	 *
	 * @param arAPDU	the DNP3 message as an APDU instance
	 */
	virtual void ConfigureRequest(APDU& arAPDU) = 0;

	/**
	 * Handler for non-FIN responses, performs common validation and
	 * delegates to _OnPartialResponse().
	 *
	 * @param arAPDU	the DNP3 message as an APDU instance
	 *
	 * @return			TaskResult enumeration
	 */
	TaskResult OnPartialResponse(const APDU& arAPDU);

	/**
	 * Handler for FIN responses, performs common validation and delegates
	 * to _OnFinalResponse().
	 *
	 * @param arAPDU	the DNP3 message as an APDU instance
	 *
	 * @return			true if a valid response, false otherwise
	 */
	TaskResult OnFinalResponse(const APDU& arAPDU);

	/**
	 * Overridable handler for timeouts, layer closes, etc.  Subclasses
	 * that wish to handle failures of the Link Layer to deliver the
	 * message should override this function.
	 */
	virtual void OnFailure() {}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	/**
	 * Returns the name of the task.
	 *
	 * @return			the name of the task
	 */
	virtual std::string Name() const = 0;
#endif

protected:

	/**
	 * Handler for non-FIN responses.  Subclasses should override this
	 * function to provide class-specific interpretations of the behavior.
	 *
	 * @param arAPDU	the DNP3 message as an APDU instance
	 *
	 * @return			a TaskResult value as a response
	 */
	virtual TaskResult _OnPartialResponse(const APDU& arAPDU) = 0;

	/**
	 * Handler for FIN responses.  Subclasses should override this
	 * function to provide class-specific interpretations of the behavior.
	 *
	 * @param arAPDU	the DNP3 message as an APDU instance
	 *
	 * @return			a TaskResult value as a response
	 */
	virtual TaskResult _OnFinalResponse(const APDU& arAPDU) = 0;

private:

	TaskResult ProcessResult(TaskResult);

	bool ValidateIIN(const IINField& GetIIN) const;
};

/**
All non-read tasks that only return a single fragment can inherit from this task
*/
class DLL_LOCAL SingleRspBase : public MasterTaskBase
{
public:
	SingleRspBase(openpal::Logger&);
	TaskResult _OnPartialResponse(const APDU&);
};

class DLL_LOCAL SimpleRspBase : public SingleRspBase
{
public:
	SimpleRspBase(openpal::Logger&);
	TaskResult _OnFinalResponse(const APDU&);
};

} //ens ns

/* vim: set ts=4 sw=4: */

#endif
