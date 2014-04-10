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
#include <catch.hpp>


#include <opendnp3/master/AsyncTaskPeriodic.h>
#include <opendnp3/master/AsyncTaskNonPeriodic.h>
#include <opendnp3/master/AsyncTaskContinuous.h>
#include <opendnp3/master/AsyncTaskGroup.h>

#include "MockExecutor.h"
#include "Exception.h"

#include <queue>
#include <chrono>


using namespace openpal;
using namespace opendnp3;
using namespace std::chrono;

class MockTaskHandler
{
public:

	openpal::Function1<AsyncTaskBase*> GetHandler()
	{
		auto lambda = [this](ITask * pTask) { this->OnTask(pTask); };
		return Bind1<AsyncTaskBase*>(lambda);
	}

	size_t Size()
	{
		return mTasks.size();
	}

	ITask* Front()
	{
		return (Size() > 0) ? mTasks.front() : nullptr;
	}

	void Pop()
	{
		mTasks.pop_front();
	}

	void Complete(bool aSuccess)
	{
		ITask* p = mTasks.front();
		Pop();
		p->OnComplete(aSuccess);
	}

private:

	std::deque<ITask*> mTasks;

	void OnTask(ITask* apTask)
	{
		mTasks.push_back(apTask);
	}
};

#define SUITE(name) "AsyncTaskSuite - " name

TEST_CASE(SUITE("DependencyAnalysis"))
{
	MockTaskHandler mth;
	MockExecutor exe;

	AsyncTaskGroup group(&exe);

	AsyncTaskBase* pT1 = group.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 0, mth.GetHandler());
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 0, mth.GetHandler());
	AsyncTaskBase* pT3 = group.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 0, mth.GetHandler());

	// try self-assignemt
	REQUIRE_FALSE(pT1->AddDependency(pT1));

	REQUIRE(!pT2->IsDependency(pT1));
	pT2->AddDependency(pT1);
	REQUIRE(pT2->IsDependency(pT1));

	REQUIRE(!pT3->IsDependency(pT2));
	pT3->AddDependency(pT2);
	REQUIRE(pT3->IsDependency(pT2));

	//check that dependencies are transitive
	REQUIRE(pT3->IsDependency(pT1));

	//try to create some circular dependencies
	REQUIRE_FALSE(pT1->AddDependency(pT2));
	REQUIRE_FALSE(pT1->AddDependency(pT3));
}

TEST_CASE(SUITE("ContinousTask"))
{
	MockTaskHandler mth;
	MockExecutor exe;

	AsyncTaskGroup group(&exe);

	AsyncTaskContinuous* pT1 = group.AddContinuous(0, mth.GetHandler());
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 1, mth.GetHandler());

	pT1->Enable();
	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT1);
	group.Enable();
	mth.Complete(true);

	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT2);
	mth.Complete(true);

	for(size_t i = 0; i < 5; ++i)
	{
		REQUIRE(mth.Size() ==  1);
		REQUIRE(mth.Front() ==  pT1);
		mth.Complete(true);
	}

	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT1);
	pT1->Disable();

}

// Two groups that execute independently of one another
TEST_CASE(SUITE("DecoupledGroupsMode"))
{
	MockTaskHandler mth;
	MockExecutor exe;

	AsyncTaskGroup group1(&exe);
	AsyncTaskGroup group2(&exe);

	AsyncTaskBase* pT1 = group1.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 0, mth.GetHandler());
	AsyncTaskBase* pT2 = group2.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 0, mth.GetHandler());

	group1.Enable();
	REQUIRE(mth.Size() ==  1);
	group2.Enable();

	REQUIRE(mth.Size() ==  2);
	REQUIRE(mth.Front() ==  pT1);
	mth.Pop();
	REQUIRE(mth.Front() ==  pT2);
}

TEST_CASE(SUITE("NonPeriodic"))
{
	MockTaskHandler mth;

	MockExecutor exe;

	AsyncTaskGroup group(&exe);


	AsyncTaskBase* pT1 = group.Add(TimeDuration::Min(), TimeDuration::Milliseconds(100), 0, mth.GetHandler()); //non-periodic task
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Milliseconds(2000), TimeDuration::Milliseconds(100), 0, mth.GetHandler());

	group.Enable();

	//complete both the tasks
	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT1);
	mth.Complete(true);
	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT2);
	mth.Complete(true);

	// a timer should be registered for the next periodic task execution
	REQUIRE(exe.NumActive() ==  1);
	REQUIRE(mth.Size() ==  0);
	exe.AdvanceTime(TimeDuration::Seconds(2));
	REQUIRE(exe.DispatchOne());
	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT2);
}

TEST_CASE(SUITE("DemandPeriodicTaskWhileNotExecuting"))
{
	MockTaskHandler mth;

	MockExecutor exe;

	AsyncTaskGroup group(&exe);

	AsyncTaskBase* pT1 = group.Add(TimeDuration::Milliseconds(2000), TimeDuration::Milliseconds(100), 0, mth.GetHandler());

	group.Enable();

	//complete both the tasks
	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT1);
	mth.Complete(true);
	REQUIRE(mth.Size() ==  0);

	pT1->Demand();
	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT1);
}

TEST_CASE(SUITE("DemandPeriodicTaskWhileExecuting"))
{
	MockTaskHandler mth;

	MockExecutor exe;

	AsyncTaskGroup group(&exe);

	AsyncTaskBase* pT1 = group.Add(TimeDuration::Milliseconds(2000), TimeDuration::Milliseconds(100), 0, mth.GetHandler());

	group.Enable();

	//complete both the tasks
	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT1);
	pT1->Demand();
	mth.Complete(true);
	REQUIRE(mth.Size() ==  0);
}


TEST_CASE(SUITE("PriorityBreaksTies"))
{
	MockTaskHandler mth;
	MockExecutor exe;

	AsyncTaskGroup group(&exe);

	group.Add(TimeDuration::Milliseconds(100), TimeDuration::Milliseconds(100), 0, mth.GetHandler());
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Milliseconds(100), TimeDuration::Milliseconds(100), 1, mth.GetHandler()); // higher priority

	group.Enable();

	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT2);
}

TEST_CASE(SUITE("DependenciesEnforced"))
{
	MockTaskHandler mth;
	MockExecutor exe;

	AsyncTaskGroup group(&exe);
	AsyncTaskBase* pT1 = group.Add(TimeDuration::Milliseconds(100), TimeDuration::Milliseconds(100), 0, mth.GetHandler());
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Milliseconds(100), TimeDuration::Milliseconds(100), 0, mth.GetHandler());

	pT1->AddDependency(pT2); // T1 depends on T2

	group.Enable();

	REQUIRE(mth.Size() ==  1);
	REQUIRE(mth.Front() ==  pT2);
}

TEST_CASE(SUITE("TimerUsage"))
{
	MockTaskHandler mth;
	MockExecutor exe;


	AsyncTaskGroup group(&exe);
	AsyncTaskBase* pT1 = group.Add(TimeDuration::Milliseconds(1000), TimeDuration::Milliseconds(100), 0, mth.GetHandler());
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Milliseconds(1500), TimeDuration::Milliseconds(100), 0, mth.GetHandler());

	group.Enable();

	// complete the two periodic tasks
	REQUIRE(mth.Front() ==  pT1); mth.Complete(true);
	REQUIRE(mth.Front() ==  pT2); mth.Complete(true);

	//if you disptach the call back early, nothing should happen except a new timer
	REQUIRE(exe.DispatchOne());

	exe.AdvanceTime(TimeDuration::Milliseconds(1001));
	REQUIRE(exe.DispatchOne());
	REQUIRE(mth.Front() ==  pT1); mth.Complete(true);

	exe.AdvanceTime(TimeDuration::Milliseconds(500));
	REQUIRE(exe.DispatchOne());
	REQUIRE(mth.Front() ==  pT2); mth.Complete(true);
}


