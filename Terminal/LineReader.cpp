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

