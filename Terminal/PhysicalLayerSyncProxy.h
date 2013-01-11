
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
#ifndef __PHYSICAL_LAYER_SYNC_PROXY_H_
#define __PHYSICAL_LAYER_SYNC_PROXY_H_

#include <APL/Thread.h>
#include <APL/Threadable.h>
#include <APL/PhysicalLayerAsyncBase.h>

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <boost/system/error_code.hpp>

namespace boost
{
namespace asio
{
class io_service;
}
}

namespace apl
{

class Logger;

/** Simple thread object that allows for non-blocking read of std input.
*/
class PhysicalLayerSyncProxy : public PhysicalLayerAsyncBase, private Threadable
{
public:
	PhysicalLayerSyncProxy(Logger*, boost::asio::io_service*);

	void DoOpen();
	void DoClose();
	void DoAsyncRead(uint8_t*, size_t);
	void DoAsyncWrite(const uint8_t*, size_t);

	static const boost::system::error_code mSuccessCode;
	static const boost::system::error_code mErrorCode;

protected:

	// Blocking read defined in base class
	virtual std::string Read() = 0;

	// Blocking write defined in base class
	virtual void Write(const std::string&) = 0;



private:

	void SignalStop();

	void Run();
	void Reset();
	void CheckForRead();

	boost::asio::io_service* mpService;

	bool mReading;
	size_t mNumToRead;
	uint8_t* mpBuffer;

	std::mutex mMutex;
	std::condition_variable mCondition;
	std::deque<std::string> mLineQueue;
	Thread mThread;

};
}

#endif

