#ifndef __I_OUTSTATION_H_
#define __I_OUTSTATION_H_

#include "IStack.h"

namespace opendnp3
{

class IDataObserver;

/**
* Interface representing a running outstation.
* To get a data observer interface to load measurements on the outstation:-
\code
	IDataObserver* pDataObserver = pOutstation->GetDataObserver()
\endcode
*/
class IOutstation : public IStack
{
public:
	IOutstation(openpal::Logger& arLogger, std::function<void (bool)> aEnableDisableFunc): 
		IStack(arLogger,aEnableDisableFunc)	
	{}

	virtual ~IOutstation() {}

	virtual void SetNeedTimeIIN() = 0;

	/**
	* Get a data observer interface to load measurements on the outstation
	* @return Inteface used to load measurements into the outstation
	*/
	virtual IDataObserver* GetDataObserver() = 0;
};

}

#endif
