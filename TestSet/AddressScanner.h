
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
#ifndef __ADDRESS_SCANNER_H_
#define __ADDRESS_SCANNER_H_

#include <APLXML/PhysicalLayerManagerXML.h>
#include <APL/IOServiceThread.h>
#include <APL/ASIOExecutor.h>
#include <APL/IOService.h>
#include <APL/Loggable.h>

#include <DNP3/LinkLayerRouter.h>
#include <DNP3/ILinkContext.h>
#include <DNP3/LinkFrame.h>

namespace APLXML_MTS
{
class MasterTestSet_t;
}
namespace apl
{
class Logger;
}
namespace boost
{
namespace asio
{
class io_service;
}
}

namespace apl
{
namespace dnp
{

class AddressScanner : private Loggable, public ILinkContext
{
public:
	AddressScanner(Logger* apLogger, const APLXML_MTS::MasterTestSet_t& cfg, uint16_t start, uint16_t stop);

	void Run();

	void OnLowerLayerUp();
	void OnLowerLayerDown();

	void Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);

	//	Pri to Sec

	void TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc);
	void ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength);
	void UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength);

private:

	void OnTimeout();

	void NextFrame();

	apl::IOService mService;
	apl::xml::PhysicalLayerManagerXML manager;
	boost::asio::strand mStrand;
	ASIOExecutor mExecutor;
	IOServiceThread mThread;
	dnp::LinkLayerRouter mRouter;
	LinkFrame mFrame;
	ITimer* mpTimer;
	uint16_t mMasterAddr;
	millis_t mScanTimeout;

	uint16_t mCurrent;
	uint16_t mStop;
};

}
}

#endif
