
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
#include "MockPhysicalLayerAsyncTS.h"

#include <APLTestTools/MockExecutor.h>
#include <APL/Logger.h>
#include <APL/LoggableMacros.h>

#include <chrono>
#include <functional>

using namespace boost::system;
using namespace std::chrono;

namespace apl
{

MockPhysicalLayerAsyncTS::MockPhysicalLayerAsyncTS(Logger* apLogger, MockExecutor* apExecutor) :
	PhysicalLayerAsyncBase(apLogger),
	mpExecutor(apExecutor),
	mpOpenTimer(NULL),
	mErrorCode(errc::permission_denied, get_generic_category()),
	mWriteBuffer(1024)
{
	this->Reset();
}

void MockPhysicalLayerAsyncTS::WriteToLayer(const uint8_t* apData, size_t aNumBytes)
{
	memcpy(mWriteBuffer.WriteBuff(), apData, aNumBytes);
	mWriteBuffer.AdvanceWrite(aNumBytes);
	this->CheckForRead();
}

void MockPhysicalLayerAsyncTS::Advance()
{
	while(mpExecutor->DispatchOne());
}

void MockPhysicalLayerAsyncTS::Reset()
{
	mpBuff = NULL;
	mNumBytes = 0;
}

void MockPhysicalLayerAsyncTS::DoOpen()
{
	mpOpenTimer = mpExecutor->Start(milliseconds(2000), std::bind(&MockPhysicalLayerAsyncTS::OnOpenCallback, this, mSuccessCode));
}

//we might have outstanding
void MockPhysicalLayerAsyncTS::DoClose()
{
	this->Reset();

	if(mState.mReading) {
		mpExecutor->Post(std::bind(&MockPhysicalLayerAsyncTS::OnReadCallback, this, mErrorCode, mpBuff, 0));
	}

	if(mState.mWriting) {
		mpExecutor->Post(std::bind(&MockPhysicalLayerAsyncTS::OnWriteCallback, this, mErrorCode, 0));
	}
}

void MockPhysicalLayerAsyncTS::DoOpeningClose()
{
	mpOpenTimer->Cancel();
	mpExecutor->Post(std::bind(&MockPhysicalLayerAsyncTS::OnOpenCallback, this, mErrorCode));
}


void MockPhysicalLayerAsyncTS::DoOpenSuccess()
{
	LOG_BLOCK(LEV_INFO, "Open success");
}

void MockPhysicalLayerAsyncTS::DoOpenFailure() {}

void MockPhysicalLayerAsyncTS::DoAsyncRead(uint8_t* apBuff, size_t aNumBytes)
{
	mpBuff = apBuff;
	mNumBytes = aNumBytes;
	mpExecutor->Post(std::bind(&MockPhysicalLayerAsyncTS::CheckForRead, this));
}

void MockPhysicalLayerAsyncTS::DoAsyncWrite(const uint8_t* apData, size_t aNumBytes)
{
	this->WriteToBuffer(apData, aNumBytes); //record to BufferTestObject
	mpExecutor->Post(std::bind(&MockPhysicalLayerAsyncTS::OnWriteCallback, this, mSuccessCode, aNumBytes));
}

void MockPhysicalLayerAsyncTS::CheckForRead()
{
	if(mNumBytes > 0 && mWriteBuffer.NumReadBytes() > 0) {
		size_t min = mNumBytes > mWriteBuffer.NumReadBytes() ? mWriteBuffer.NumReadBytes() : mNumBytes;
		memcpy(mpBuff, mWriteBuffer.ReadBuff(), min);
		mNumBytes = 0;
		this->OnReadCallback(mSuccessCode, mpBuff, min);
		mWriteBuffer.AdvanceRead(min);
		mWriteBuffer.Shift();
	}
}

} //end namespace
