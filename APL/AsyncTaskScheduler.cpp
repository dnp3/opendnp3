
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
#include "AsyncTaskScheduler.h"

#include "AsyncTaskGroup.h"
#include "AsyncTaskBase.h"
#include "Exception.h"
#include "Location.h"

#include <vector>

using namespace std;

namespace apl
{

AsyncTaskScheduler::AsyncTaskScheduler(IExecutor* apExecutor, ITimeSource* apTimeSrc) :
	mpExecutor(apExecutor),
	mpTimeSrc(apTimeSrc)
{

}

AsyncTaskScheduler::~AsyncTaskScheduler()
{
	for(auto pTaskGroup: mGroupSet) delete pTaskGroup;
}

AsyncTaskGroup* AsyncTaskScheduler::CreateNewGroup()
{
	std::unique_lock<std::mutex> lock(mMutex);
	AsyncTaskGroup* pGroup = new AsyncTaskGroup(mpExecutor, mpTimeSrc);
	mGroupSet.insert(pGroup);
	return pGroup;
}

void AsyncTaskScheduler::ReleaseGroup(AsyncTaskGroup* apGroup)
{
	std::unique_lock<std::mutex> lock(mMutex);
	GroupSet::iterator i = mGroupSet.find(apGroup);
	if( i != mGroupSet.end() ) {
		delete *i;
		mGroupSet.erase(i);
	}
	else throw ArgumentException(LOCATION, "Group not associated with this scheduler");
}

} //end ns
