
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
#include "AddressScanner.h"

#include <APL/Logger.h>
#include <APL/LoggableMacros.h>

#include <APLXML/XMLConversion.h>
#include <XMLBindings/APLXML_MTS.h>

namespace apl
{
namespace dnp
{

AddressScanner::AddressScanner(Logger* apLogger, const APLXML_MTS::MasterTestSet_t& cfg, uint16_t start, uint16_t stop) :
	Loggable(apLogger),
	mService(),
	manager(apLogger, mService.Get(), &cfg.PhysicalLayerList, xml::Convert(cfg.Log.Filter)),
	mStrand(*mService.Get()),
	mExecutor(&mStrand),
	mThread(apLogger->GetSubLogger("ioservice"), mService.Get()),
	mRouter(apLogger, manager.AcquireLayer(cfg.PhysicalLayer), &mExecutor, 1000),
	mpTimer(NULL),
	mMasterAddr(cfg.Master.Stack.LinkLayer.LocalAddress),
	mScanTimeout(cfg.Master.Stack.LinkLayer.AckTimeoutMS),
	mCurrent(start),
	mStop(stop)
{
	LinkRoute route(cfg.Master.Stack.LinkLayer.RemoteAddress, cfg.Master.Stack.LinkLayer.LocalAddress);
	mRouter.AddContext(this, route);
}

void AddressScanner::OnLowerLayerUp()
{
	this->NextFrame();
}

void AddressScanner::OnLowerLayerDown()
{

}

void AddressScanner::OnTimeout()
{
	LOG_BLOCK(LEV_INFO, "Scan timed out for address: " << mFrame.GetDest());
	mpTimer = NULL;
	this->NextFrame();
}

void AddressScanner::NextFrame()
{
	if(mCurrent >= mStop) mRouter.Shutdown();
	else {
		mpTimer = mExecutor.Start(std::chrono::milliseconds(mScanTimeout), std::bind(&AddressScanner::OnTimeout, this));
		mFrame.FormatResetLinkStates(true, mCurrent, mMasterAddr);
		++mCurrent;
		mRouter.Transmit(mFrame);
	}
}

void AddressScanner::Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	LOG_BLOCK(LEV_EVENT, "Received acknowledgement from address: " << aSrc);
	mRouter.Shutdown();
	if(mpTimer != NULL) mpTimer->Cancel();
}

void AddressScanner::Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) {}
void AddressScanner::LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) {}
void AddressScanner::NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) {}

void AddressScanner::TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc) {}
void AddressScanner::ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc) {}
void AddressScanner::RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc) {}
void AddressScanner::ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength) {}
void AddressScanner::UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength) {}

void AddressScanner::Run()
{
	mRouter.Start();

	LOG_BLOCK(LEV_INFO, "Scanning from " << mCurrent << " to " << mStop);
	mThread.Run();
	LOG_BLOCK(LEV_INFO, "Scan complete...");
}

}
}

