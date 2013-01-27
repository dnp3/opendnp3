#include "com_automatak_dnp3_impl_DNP3ManagerImpl.h"


#include <DNP3/DNP3Manager.h>
#include <DNP3/IChannel.h>
#include <APL/LogToStdio.h>
#include <APL/LogTypes.h>
#include <APL/SimpleDataObserver.h>
#include <thread>

using namespace apl;
using namespace apl::dnp;

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_DNP3ManagerImpl_create_1native_1manager
  (JNIEnv *, jobject)
{
	auto pMgr = new DNP3Manager(std::thread::hardware_concurrency());
	pMgr->AddLogSubscriber(LogToStdio::Inst());
	auto pChannel = pMgr->AddTCPClient("client", LEV_INFO, 3000, "127.0.0.1", 20000);
	auto pMaster = pChannel->AddMaster("master", LEV_INFO, PrintingDataObserver::Inst(), MasterStackConfig());
	return (jlong) (pMgr);
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DNP3ManagerImpl_destroy_1native_1manager
  (JNIEnv *, jobject, jlong apManager)
{
	delete (DNP3Manager*) apManager;
}

