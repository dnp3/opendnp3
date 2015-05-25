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
#ifndef OPENDNP3_MASTERACTIONS_H
#define OPENDNP3_MASTERACTIONS_H

#include "MasterState.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/container/ReadBufferView.h>

#include "opendnp3/master/MasterState.h"

namespace opendnp3
{

	class MasterActions : private openpal::Uncopyable
	{
	public:

		static void StartTask(MasterState& mstate, IMasterTask& task);

		static void QueueConfirm(MasterState& mstate, const APDUHeader& header);

		static void StartResponseTimer(MasterState& mstate);

		static void PostCheckForTask(MasterState& mstate);

		static void ReleaseActiveTask(MasterState& mstate);		

		static bool CheckConfirmTransmit(MasterState& mstate);		

	private:		

		static void ProcessResponseTimeout(MasterState& mstate);
	
		static void Transmit(MasterState& mstate, const openpal::ReadBufferView& data);
	};

}

#endif
