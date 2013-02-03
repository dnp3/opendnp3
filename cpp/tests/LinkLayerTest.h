
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
#ifndef __LINK_LAYER_TEST_H_
#define __LINK_LAYER_TEST_H_

#include "LogTester.h"
#include "MockUpperLayer.h"
#include "MockExecutor.h"

#include <opendnp3/LinkLayer.h>
#include <opendnp3/ILinkRouter.h>

namespace opendnp3
{

class LinkLayerTest : public LogTester, public ILinkRouter
{
public:

	LinkLayerTest(LinkConfig arCfg = DefaultConfig(), FilterLevel aLevel = LEV_WARNING, bool aImmediate = false);

	//ILinkRouter interface
	void Transmit(const LinkFrame&);

	static LinkConfig DefaultConfig();

	MockExecutor mts;
	MockUpperLayer upper;
	LinkLayer link;

	LinkFrame mLastSend;
	size_t mNumSend;
};

}

#endif

