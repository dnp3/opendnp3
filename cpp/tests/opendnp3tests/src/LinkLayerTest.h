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
#ifndef __LINK_LAYER_TEST_H_
#define __LINK_LAYER_TEST_H_

#include "LogTester.h"
#include "MockTransportLayer.h"
#include "MockExecutor.h"

#include <opendnp3/link/LinkLayer.h>
#include <opendnp3/link/ILinkRouter.h>
#include <opendnp3/LogLevels.h>

#include <queue>

namespace opendnp3
{

class LinkLayerTest : public ILinkRouter
{
public:

	LinkLayerTest(LinkConfig arCfg = DefaultConfig());

	//ILinkRouter interface
	virtual void BeginTransmit(const openpal::ReadOnlyBuffer& buffer, ILinkContext* pContext) override final;

	static LinkConfig DefaultConfig();

	LogTester log;
	MockExecutor exe;
	LinkLayer link;
	MockTransportLayer upper;

	openpal::ReadOnlyBuffer lastWrite;
	uint32_t numWrites;
};

}

#endif

