
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __MASTER_DEMO_H_
#define __MASTER_DEMO_H_

#include <APL/CommandQueue.h>
#include <APL/FlexibleDataObserver.h>
#include <APL/IOService.h>
#include <APL/IOServiceThread.h>
#include <APL/IPhysicalLayerAsync.h>
#include <APL/Log.h>
#include <APL/LogToStdio.h>
#include <APL/Loggable.h>
#include <APL/PostingNotifierSource.h>
#include <APL/QueueingFDO.h>
#include <APL/ASIOExecutor.h>

#include <DNP3/AsyncStackManager.h>
#include <DNP3/MasterStackConfig.h>

namespace apl { namespace dnp {

/**
 * This class takes care of all the plumbing and bingings between
 * application code and the stack.
 */
class MasterDemoBase
	: private IOService
	, public IOServiceThread
	, public IResponseAcceptor
{
public:
	/**
	 * Default constructor for the object.
	 */
	MasterDemoBase(Logger* apLogger);

	/**
	 * Tells the io_service to exit.
	 */
	void Shutdown();

	/**
	 * Returns the data observer for the stack.
	 */
	IDataObserver* GetDataObserver()
	{
		return &mFDO;
	}

	ICommandAcceptor* GetCommandAcceptor()
	{
		return mpCommandAcceptor;
	}

	void SetCommandAcceptor(ICommandAcceptor* apCommandAcceptor)
	{
		mpCommandAcceptor = apCommandAcceptor;
	}

	void AcceptResponse(const CommandResponse& aResponse,
			int aSequence);

private:
	/**
	 * A notifier that gets triggered when data updates are
	 * received.
	 */
	void OnDataUpdate();

	/**
	 * The QueueingFDO object which is responsible for keeping track
	 * of new data updates.
	 */
	QueueingFDO mFDO;

	/**
	 * The ICommandAcceptor instance created for the stack.
	 */
	ICommandAcceptor* mpCommandAcceptor;

	PostingNotifierSource mPostSource;	// bridge between Notifiers and IO services that marshalls method calls between threads
	boost::asio::strand mStrand;		
	ASIOExecutor mExecutor;				// boost timer source, interface into the io service system
	ITimer* mpInfiniteTimer;			// timer used to keep the boost io service alive

	int16_t mTestCounter;
};


/**
 * Something, not sure what yet.
 */
class MasterDemoApp : public MasterDemoBase, public IStackObserver
{
public:
	MasterDemoApp(Logger* apLogger);

	void OnStateChange(StackStates aState);
};

}} // end namespaces apl and apl::dnp

#endif

