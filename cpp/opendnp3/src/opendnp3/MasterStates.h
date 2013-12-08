/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __MASTER_STATES_H_
#define __MASTER_STATES_H_

#include <string>

#include <opendnp3/Singleton.h>
#include <opendnp3/ObjectInterfaces.h>


namespace opendnp3
{

class ITaskCompletion;
class ITask;
class Master;
class MasterTaskBase;
class APDU;

class AMS_Base
{
public:

	// called when a new task should be started
	virtual void StartTask(Master* c, ITask*, MasterTaskBase*);

	/* Events from application layer */

	virtual void OnLowerLayerUp(Master*);
	virtual void OnLowerLayerDown(Master*);

	virtual void OnSendSuccess(Master*);
	virtual void OnFailure(Master*);

	virtual void OnPartialResponse(Master*, const APDU&);
	virtual void OnFinalResponse(Master*, const APDU&);

	virtual void OnUnsolResponse(Master*, const APDU&);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	virtual std::string Name() const = 0;
#endif

protected:

	void ChangeState(Master*, AMS_Base*);
	void ChangeTask(Master*, MasterTaskBase*);
};

/* AMS_Closed */

class AMS_Closed : public AMS_Base
{
	MACRO_STATE_SINGLETON_INSTANCE(AMS_Closed);

	void OnLowerLayerUp(Master*);
};

/* AMS_OpenBase */

class AMS_OpenBase : public AMS_Base
{
public:
	void OnUnsolResponse(Master*, const APDU&);
	virtual void OnLowerLayerDown(Master* c);
};

/* AMS_Idle */

// The only state from which a task can be started
class AMS_Idle : public AMS_OpenBase
{
	MACRO_STATE_SINGLETON_INSTANCE(AMS_Idle);

	void StartTask(Master* c, ITask*, MasterTaskBase*);
};

/* AMS_Waiting */

// Wait for responses or failure. Manipulates
// the current task instance.
class AMS_Waiting : public AMS_OpenBase
{
	MACRO_STATE_SINGLETON_INSTANCE(AMS_Waiting);

	void OnFailure(Master*);
	void OnPartialResponse(Master*, const APDU&);
	void OnFinalResponse(Master*, const APDU&);

	void OnLowerLayerDown(Master* c);
};

} //ens ns

#endif
