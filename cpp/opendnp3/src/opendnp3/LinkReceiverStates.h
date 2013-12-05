#ifndef __LINK_RECEIVER_STATES_H_
#define __LINK_RECEIVER_STATES_H_


#include <string>

#include <opendnp3/Uncopyable.h>
#include <opendnp3/Singleton.h>
#include <openpal/Visibility.h>

#include "LinkLayerReceiver.h"

namespace opendnp3
{

/** Base class for the receiver state */
class DLL_LOCAL LRS_Base : private Uncopyable
{
public:

	virtual ~LRS_Base() {}

	/** Perform a bit of work towards reading a frame.
		@return True if the function can be called again to make further progress */
	virtual bool Parse(LinkLayerReceiver*);

	/** @return True if the receiver has a complete frame */
	virtual bool HasFrame() {
		return false;
	}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	/** Implemented by the inherited state
		@return Name of the state */
	virtual std::string Name() const = 0;
#endif

};

#define MACRO_LRS_STATE(name, body) \
class DLL_LOCAL name : public LRS_Base \
{ \
	MACRO_STATE_SINGLETON_INSTANCE(name) \
	body \
};

/** @section DESCRIPTION Defines actions for the initial state of the receiver */
MACRO_LRS_STATE(LRS_Sync,
                bool Parse(LinkLayerReceiver*);
               )

MACRO_LRS_STATE(LRS_Header,
                bool Parse(LinkLayerReceiver*);
               )

MACRO_LRS_STATE(LRS_Body,
                bool Parse(LinkLayerReceiver*);
               )

}

#endif
