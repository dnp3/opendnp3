
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
#include "LineReader.h"

#include <APL/IPhysicalLayerAsync.h>

using namespace std;
using namespace std::chrono;

namespace apl
{
LineReader::LineReader(Logger* apLogger, IPhysicalLayerAsync* apPhysical, IExecutor* apExecutor, size_t aBuffSize) :
	Loggable(apLogger),
	PhysicalLayerMonitor(apLogger, apPhysical, apExecutor, seconds(5), seconds(5)),
	mBuffer(aBuffSize)
{
	this->Reset();
}

void LineReader::OnPhysicalLayerOpenSuccessCallback()
{
	this->Read();
	this->_Up();
}

void LineReader::OnPhysicalLayerCloseCallback()
{
	this->Reset();
	this->_Down();
}

void LineReader::Reset()
{
	mNumBytes = 0;
	mHasCR = false;
}

void LineReader::Read()
{
	if( (mBuffer.Size() - mNumBytes) == 0) this->Reset();
	mpPhys->AsyncRead(mBuffer + mNumBytes, mBuffer.Size() - mNumBytes);
}

void LineReader::_OnReceive(const uint8_t*, size_t aNum)
{
	mNumBytes += aNum;
	assert(mNumBytes <= mBuffer.Size());
	this->ReadBuffer();
	if(mpPhys->CanRead()) this->Read();
}

void LineReader::ReadBuffer()
{
	size_t pos = 0;
	while(pos < (mNumBytes - 1)) {
		if(mBuffer[pos] == '\r' && mBuffer[pos + 1] == '\n') {
			size_t length = pos + 2;
			size_t remain = mNumBytes - length;
			std::string s(reinterpret_cast<const char*>(mBuffer.Buffer()), length - 2);
			mNumBytes -= length;
			this->AcceptLine(s);
			if(remain > 0) {
				memmove(mBuffer, mBuffer + length, remain);
				ReadBuffer();
			}
			break;
		}
		++pos;
	}
}
}

