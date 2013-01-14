
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
#include "Threadable.h"

#include <assert.h>

namespace apl
{

Threadable::Threadable() :
	mIsExitRequested(false),
	mIsRunning(false)
{

}

Threadable::~Threadable()
{
	assert(!mIsRunning);
}

void Threadable::Start()
{
	assert(!mIsRunning);

	mIsRunning = true;

	//enforce pre and post conditions of Run()
	this->Run();

	mIsRunning = false;
}

void Threadable::RequestStop()
{
	mIsExitRequested = true;
	this->SignalStop();
}

void Threadable::SignalStop()
{

}

void Threadable::ResetStopRequest()
{
	assert(!mIsRunning);
	mIsExitRequested = false;
}

} //end namespace
