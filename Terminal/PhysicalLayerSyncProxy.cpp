#include "PhysicalLayerSyncProxy.h"

#include <boost/asio.hpp>
#include <iostream>

using namespace std;

namespace apl
{

const boost::system::error_code PhysicalLayerSyncProxy::mSuccessCode(boost::system::errc::success, boost::system::get_generic_category());
const boost::system::error_code PhysicalLayerSyncProxy::mErrorCode(boost::system::errc::permission_denied, boost::system::get_generic_category());

PhysicalLayerSyncProxy::PhysicalLayerSyncProxy(Logger* apLogger, boost::asio::io_service* apService) :
	PhysicalLayerAsyncBase(apLogger),
	mpService(apService),
	mThread(this)
{
	this->Reset();
	mThread.Start();
}

void PhysicalLayerSyncProxy::SignalStop()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mCondition.notify_all();
}

void PhysicalLayerSyncProxy::Reset()
{
	mReading = false;
	mpBuffer = NULL;
	mNumToRead = 0;
	mLineQueue.clear();
}

void PhysicalLayerSyncProxy::CheckForRead()
{
	if(mReading && mLineQueue.size() > 0) {
		std::string copy(mLineQueue.front());
		mLineQueue.pop_front();
		size_t len = copy.size();
		size_t min = len > mNumToRead ? mNumToRead : len;
		size_t remainder = len - min;
		memcpy(mpBuffer, copy.c_str(), min);
		if(remainder > 0) {
			std::string r(copy.c_str() + mNumToRead, remainder);
			mLineQueue.push_front(r);
		}
		mReading = false;
		mpService->post(std::bind(&PhysicalLayerSyncProxy::OnReadCallback, this, mSuccessCode, mpBuffer, min));
	}
}

void PhysicalLayerSyncProxy::DoOpen()
{
	this->Reset();
	mpService->post(std::bind(&PhysicalLayerSyncProxy::OnOpenCallback, this, mSuccessCode));
}

void PhysicalLayerSyncProxy::DoClose()
{

}

void PhysicalLayerSyncProxy::DoAsyncRead(uint8_t* apData, size_t aLength)
{
	std::unique_lock<std::mutex> lock(mMutex);
	mpBuffer = apData;
	mNumToRead = aLength;
	mReading = true;
	this->CheckForRead();
	if(mReading) mCondition.notify_all(); //wake up the thread to go and get more data from stdin
}

void PhysicalLayerSyncProxy::DoAsyncWrite(const uint8_t* apData, size_t aLength)
{
	const char* pBuff = reinterpret_cast<const char*>(apData);
	string s(pBuff, aLength);
	this->Write(s);
	mpService->post(std::bind(&PhysicalLayerSyncProxy::OnWriteCallback, this, mSuccessCode, aLength));
}

void PhysicalLayerSyncProxy::Run()
{
	while(!IsExitRequested()) {
		std::unique_lock<std::mutex> lock(mMutex);
		if( !mReading ) mCondition.wait(lock);
		if( mReading ) {
			mLineQueue.push_back(this->Read());
			this->CheckForRead();
		}
	}
}
}
