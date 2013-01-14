
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
#include <boost/test/unit_test.hpp>


#include <APL/AsyncTaskScheduler.h>
#include <APL/AsyncTaskPeriodic.h>
#include <APL/AsyncTaskNonPeriodic.h>
#include <APL/AsyncTaskContinuous.h>
#include <APL/AsyncTaskGroup.h>
#include <APL/Exception.h>

#include <APLTestTools/MockExecutor.h>

#include <boost/bind.hpp>
#include <queue>
#include <chrono>

using namespace apl;
using namespace boost;
using namespace std::chrono;



class MockTaskHandler
{
public:

	TaskHandler GetHandler() {
		return bind(&MockTaskHandler::OnTask, this, _1);
	}

	size_t Size() {
		return mTasks.size();
	}

	ITask* Front() {
		return (Size() > 0) ? mTasks.front() : NULL;
	}

	void Pop() {
		mTasks.pop_front();
	}

	void Complete(bool aSuccess) {
		ITask* p = mTasks.front();
		Pop();
		p->OnComplete(aSuccess);
	}

private:

	std::deque<ITask*> mTasks;

	void OnTask(ITask* apTask) {
		mTasks.push_back(apTask);
	}
};

BOOST_AUTO_TEST_SUITE(AsyncTaskSuite)

BOOST_AUTO_TEST_CASE(DependencyAnalysis)
{
	MockTaskHandler mth;
	MockExecutor exe;
	AsyncTaskScheduler ats;

	AsyncTaskGroup* pGroup = ats.CreateNewGroup(&exe);

	AsyncTaskBase* pT1 = pGroup->Add(1000, 1000, 0, mth.GetHandler());
	AsyncTaskBase* pT2 = pGroup->Add(1000, 1000, 0, mth.GetHandler());
	AsyncTaskBase* pT3 = pGroup->Add(1000, 1000, 0, mth.GetHandler());

	// try self-assignemt
	BOOST_REQUIRE_THROW(pT1->AddDependency(pT1), ArgumentException);

	BOOST_REQUIRE(!pT2->IsDependency(pT1));
	pT2->AddDependency(pT1);
	BOOST_REQUIRE(pT2->IsDependency(pT1));

	BOOST_REQUIRE(!pT3->IsDependency(pT2));
	pT3->AddDependency(pT2);
	BOOST_REQUIRE(pT3->IsDependency(pT2));

	//check that dependencies are transitive
	BOOST_REQUIRE(pT3->IsDependency(pT1));

	//try to create some circular dependencies
	BOOST_REQUIRE_THROW(pT1->AddDependency(pT2), ArgumentException);
	BOOST_REQUIRE_THROW(pT1->AddDependency(pT3), ArgumentException);
}

BOOST_AUTO_TEST_CASE(ContinousTask)
{
	MockTaskHandler mth;
	MockExecutor exe;
	AsyncTaskScheduler ats;

	
	AsyncTaskGroup* pGroup = ats.CreateNewGroup(&exe);

	AsyncTaskContinuous* pT1 = pGroup->AddContinuous(0, mth.GetHandler());
	AsyncTaskBase* pT2 = pGroup->Add(1000, 1000, 1, mth.GetHandler());

	pT1->Enable();
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1);
	pGroup->Enable();
	mth.Complete(true);

	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2);
	mth.Complete(true);

	for(size_t i = 0; i < 5; ++i) {
		BOOST_REQUIRE_EQUAL(mth.Size(), 1);
		BOOST_REQUIRE_EQUAL(mth.Front(), pT1);
		mth.Complete(true);
	}

	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1);
	pT1->Disable();
	
}

// Two groups that execute independently of one another
BOOST_AUTO_TEST_CASE(DecoupledGroupsMode)
{
	MockTaskHandler mth;
	MockExecutor exe;
	AsyncTaskScheduler ats;

	
	AsyncTaskGroup* pGroup1 = ats.CreateNewGroup(&exe);
	AsyncTaskGroup* pGroup2 = ats.CreateNewGroup(&exe);

	AsyncTaskBase* pT1 = pGroup1->Add(1000, 1000, 0, mth.GetHandler());
	AsyncTaskBase* pT2 = pGroup2->Add(1000, 1000, 0, mth.GetHandler());

	pGroup1->Enable();
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	pGroup2->Enable();

	BOOST_REQUIRE_EQUAL(mth.Size(), 2);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1);
	mth.Pop();
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2);
}

BOOST_AUTO_TEST_CASE(NonPeriodic)
{
	MockTaskHandler mth;
	
	MockTimeSource fakeTime;
	fakeTime.SetToNow();
	MockExecutor exe;
	AsyncTaskScheduler ats(&fakeTime);

	
	AsyncTaskGroup* pGroup = ats.CreateNewGroup(&exe);


	AsyncTaskBase* pT1 = pGroup->Add(-1, 100, 0, mth.GetHandler()); //non-periodic task
	AsyncTaskBase* pT2 = pGroup->Add(2000, 100, 0, mth.GetHandler());

	pGroup->Enable();

	//complete both the tasks
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1);
	mth.Complete(true);
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2);
	mth.Complete(true);

	// a timer should be registered for the next periodic task execution
	BOOST_REQUIRE_EQUAL(exe.NumActive(), 1);
	BOOST_REQUIRE_EQUAL(mth.Size(), 0);
	fakeTime.Advance(milliseconds(2000));
	BOOST_REQUIRE(exe.DispatchOne());
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2);
}


BOOST_AUTO_TEST_CASE(PriorityBreaksTies)
{
	MockTaskHandler mth;
	MockExecutor exe;
	AsyncTaskScheduler ats;
	
	AsyncTaskGroup* pGroup = ats.CreateNewGroup(&exe);

	pGroup->Add(100, 100, 0, mth.GetHandler());
	AsyncTaskBase* pT2 = pGroup->Add(100, 100, 1, mth.GetHandler()); // higher priority

	pGroup->Enable();

	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2);
}

BOOST_AUTO_TEST_CASE(DependenciesEnforced)
{
	MockTaskHandler mth;
	MockExecutor exe;
	AsyncTaskScheduler ats;
	
	AsyncTaskGroup* pGroup = ats.CreateNewGroup(&exe);
	AsyncTaskBase* pT1 = pGroup->Add(100, 100, 0, mth.GetHandler());
	AsyncTaskBase* pT2 = pGroup->Add(100, 100, 0, mth.GetHandler());

	pT1->AddDependency(pT2); // T1 depends on T2

	pGroup->Enable();

	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2);
}

BOOST_AUTO_TEST_CASE(TimerUsage)
{		
	MockTaskHandler mth;
	MockTimeSource fake_time;
	MockExecutor exe;
	AsyncTaskScheduler ats(&fake_time);

	fake_time.SetToNow();
	
	AsyncTaskGroup* pGroup = ats.CreateNewGroup(&exe);
	AsyncTaskBase* pT1 = pGroup->Add(1000, 100, 0, mth.GetHandler());
	AsyncTaskBase* pT2 = pGroup->Add(1500, 100, 0, mth.GetHandler());

	pGroup->Enable();

	// complete the two periodic tasks
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1); mth.Complete(true);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2); mth.Complete(true);

	//if you disptach the call back early, nothing should happen except a new timer
	BOOST_REQUIRE(exe.DispatchOne());

	fake_time.Advance(milliseconds(1001));
	BOOST_REQUIRE(exe.DispatchOne());
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1); mth.Complete(true);

	fake_time.Advance(milliseconds(500));
	BOOST_REQUIRE(exe.DispatchOne());
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2); mth.Complete(true);
}

BOOST_AUTO_TEST_SUITE_END()
