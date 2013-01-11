
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
#ifndef __MOCK_FRAME_SINK_H_
#define __MOCK_FRAME_SINK_H_

#include <DNP3/ILinkContext.h>
#include <DNP3/LinkLayerConstants.h>
#include <APLTestTools/BufferTestObject.h>

#include <functional>
#include <queue>

namespace apl
{
namespace dnp
{

class MockFrameSink : public ILinkContext, public BufferTestObject
{
public:

	MockFrameSink();

	// ILinkContext members
	void OnLowerLayerUp();
	void OnLowerLayerDown();

	//	Sec to Pri
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

	void Reset();

	size_t mNumFrames;

	bool CheckLast(FuncCodes aCode, bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	bool CheckLastWithFCB(FuncCodes aCode, bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc);
	bool CheckLastWithDFC(FuncCodes aCode, bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);

	// Last frame information
	FuncCodes mCode;
	bool mIsMaster;
	bool mIsRcvBuffFull;
	uint16_t mSrc;
	uint16_t mDest;
	bool mFcb;

	bool mLowerOnline;

	// Add a function to execute the next time a frame is received
	// This allows us to test re-entrant behaviors
	void AddAction(std::function<void ()> aFunc);

private:

	// Executes one action, if one is available
	void ExecuteAction();

	std::deque< std::function<void ()> > mActions;

	void Update(FuncCodes aCode, bool aIsMaster, uint16_t aSrc, uint16_t aDest);
};

}
}

#endif
