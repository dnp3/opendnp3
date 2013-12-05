#include <boost/test/unit_test.hpp>


#include <opendnp3/AsyncTaskPeriodic.h>
#include <opendnp3/AsyncTaskNonPeriodic.h>
#include <opendnp3/AsyncTaskContinuous.h>
#include <opendnp3/AsyncTaskGroup.h>

#include <openpal/Exception.h>

#include "MockExecutor.h"

#include <boost/bind.hpp>
#include <queue>
#include <chrono>


using namespace openpal;
using namespace opendnp3;
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
		return (Size() > 0) ? mTasks.front() : nullptr;
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

	AsyncTaskGroup group(&exe);

	AsyncTaskBase* pT1 = group.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 0, mth.GetHandler());
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 0, mth.GetHandler());
	AsyncTaskBase* pT3 = group.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 0, mth.GetHandler());

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

	AsyncTaskGroup group(&exe);

	AsyncTaskContinuous* pT1 = group.AddContinuous(0, mth.GetHandler());
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 1, mth.GetHandler());

	pT1->Enable();
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1);
	group.Enable();
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

	AsyncTaskGroup group1(&exe);
	AsyncTaskGroup group2(&exe);

	AsyncTaskBase* pT1 = group1.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 0, mth.GetHandler());
	AsyncTaskBase* pT2 = group2.Add(TimeDuration::Seconds(1), TimeDuration::Seconds(1), 0, mth.GetHandler());

	group1.Enable();
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	group2.Enable();

	BOOST_REQUIRE_EQUAL(mth.Size(), 2);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1);
	mth.Pop();
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2);
}

BOOST_AUTO_TEST_CASE(NonPeriodic)
{
	MockTaskHandler mth;

	MockExecutor exe;

	AsyncTaskGroup group(&exe);


	AsyncTaskBase* pT1 = group.Add(TimeDuration::Min(), TimeDuration::Milliseconds(100), 0, mth.GetHandler()); //non-periodic task
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Milliseconds(2000), TimeDuration::Milliseconds(100), 0, mth.GetHandler());

	group.Enable();

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
	exe.AdvanceTime(TimeDuration::Seconds(2));
	BOOST_REQUIRE(exe.DispatchOne());
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2);
}

BOOST_AUTO_TEST_CASE(DemandPeriodicTaskWhileNotExecuting)
{
	MockTaskHandler mth;

	MockExecutor exe;

	AsyncTaskGroup group(&exe);
	
	AsyncTaskBase* pT1 = group.Add(TimeDuration::Milliseconds(2000), TimeDuration::Milliseconds(100), 0, mth.GetHandler());

	group.Enable();
	
	//complete both the tasks
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1);
	mth.Complete(true);
	BOOST_REQUIRE_EQUAL(mth.Size(), 0);

	pT1->Demand();
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1);	
}

BOOST_AUTO_TEST_CASE(DemandPeriodicTaskWhileExecuting)
{
	MockTaskHandler mth;

	MockExecutor exe;

	AsyncTaskGroup group(&exe);
	
	AsyncTaskBase* pT1 = group.Add(TimeDuration::Milliseconds(2000), TimeDuration::Milliseconds(100), 0, mth.GetHandler());

	group.Enable();
	
	//complete both the tasks
	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1);
	pT1->Demand();
	mth.Complete(true);
	BOOST_REQUIRE_EQUAL(mth.Size(), 0);
}


BOOST_AUTO_TEST_CASE(PriorityBreaksTies)
{
	MockTaskHandler mth;
	MockExecutor exe;

	AsyncTaskGroup group(&exe);

	group.Add(TimeDuration::Milliseconds(100), TimeDuration::Milliseconds(100), 0, mth.GetHandler());
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Milliseconds(100), TimeDuration::Milliseconds(100), 1, mth.GetHandler()); // higher priority

	group.Enable();

	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2);
}

BOOST_AUTO_TEST_CASE(DependenciesEnforced)
{
	MockTaskHandler mth;
	MockExecutor exe;

	AsyncTaskGroup group(&exe);
	AsyncTaskBase* pT1 = group.Add(TimeDuration::Milliseconds(100), TimeDuration::Milliseconds(100), 0, mth.GetHandler());
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Milliseconds(100), TimeDuration::Milliseconds(100), 0, mth.GetHandler());

	pT1->AddDependency(pT2); // T1 depends on T2

	group.Enable();

	BOOST_REQUIRE_EQUAL(mth.Size(), 1);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2);
}

BOOST_AUTO_TEST_CASE(TimerUsage)
{
	MockTaskHandler mth;
	MockExecutor exe;


	AsyncTaskGroup group(&exe);
	AsyncTaskBase* pT1 = group.Add(TimeDuration::Milliseconds(1000), TimeDuration::Milliseconds(100), 0, mth.GetHandler());
	AsyncTaskBase* pT2 = group.Add(TimeDuration::Milliseconds(1500), TimeDuration::Milliseconds(100), 0, mth.GetHandler());

	group.Enable();

	// complete the two periodic tasks
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1); mth.Complete(true);
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2); mth.Complete(true);

	//if you disptach the call back early, nothing should happen except a new timer
	BOOST_REQUIRE(exe.DispatchOne());

	exe.AdvanceTime(TimeDuration::Milliseconds(1001));
	BOOST_REQUIRE(exe.DispatchOne());
	BOOST_REQUIRE_EQUAL(mth.Front(), pT1); mth.Complete(true);

	exe.AdvanceTime(TimeDuration::Milliseconds(500));
	BOOST_REQUIRE(exe.DispatchOne());
	BOOST_REQUIRE_EQUAL(mth.Front(), pT2); mth.Complete(true);
}

BOOST_AUTO_TEST_SUITE_END()
