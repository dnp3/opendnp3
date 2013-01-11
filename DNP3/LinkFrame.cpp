
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
#include "LinkFrame.h"

#include "DNPCrc.h"

#include <assert.h>
#include <sstream>
#include <APL/Util.h>

using namespace std;

namespace apl
{
namespace dnp
{

LinkFrame::LinkFrame() :
	mIsComplete(false),
	mSize(0)
{

}

LinkFrame::~LinkFrame()
{

}

ostream& operator<<(ostream& output, const LinkFrame& f)
{
	output << f.ToString();
	return output;  // for multiple << operators.
}

bool LinkFrame::operator==(const LinkFrame& arRHS) const
{
	if(!this->IsComplete() || !arRHS.IsComplete()) return false;

	if(this->GetSize() != arRHS.GetSize()) return false;

	for(size_t i = 0; i < this->GetSize(); ++i)
		if(this->GetBuffer()[i] != arRHS.GetBuffer()[i]) return false;

	return true;
}

void LinkFrame::ReadUserData(const uint8_t* apSrc, uint8_t* apDest, size_t aLength)
{
	if(aLength == 0) return;	//base case of recursion
	size_t max = LS_DATA_BLOCK_SIZE;
	size_t num = (aLength <= max) ? aLength : max;
	size_t num_with_crc = num + 2;
	memcpy(apDest, apSrc, num);
	ReadUserData(apSrc + num_with_crc, apDest + num, aLength - num); //tail recursive
}

bool LinkFrame::ValidateHeaderCRC() const
{
	return UInt16LE::Read(mpBuffer + LI_CRC) == DNPCrc::CalcCrc(mpBuffer, LI_CRC);
}

bool LinkFrame::ValidateBodyCRC() const
{
	return ValidateBodyCRC(mpBuffer + LS_HEADER_SIZE, mHeader.GetLength() - LS_MIN_LENGTH);
}

bool LinkFrame::ValidateBodyCRC(const uint8_t* apBody, size_t aLength)
{
	if(aLength == 0) return true;	//base case of recursion
	size_t max = LS_DATA_BLOCK_SIZE ;
	size_t num = (aLength <= max) ? aLength : max;
	if(!DNPCrc::IsCorrectCRC(apBody, num)) return false;
	else return ValidateBodyCRC(apBody + num + 2, aLength - num); //tail recursive
}

size_t LinkFrame::CalcFrameSize(size_t aDataLength)
{
	assert(aDataLength <= LS_MAX_USER_DATA_SIZE);

	size_t ret = LS_HEADER_SIZE;

	if(aDataLength > 0) {
		size_t mod16 = aDataLength % LS_DATA_BLOCK_SIZE;
		ret += (aDataLength / LS_DATA_BLOCK_SIZE) * LS_DATA_PLUS_CRC_SIZE; //complete blocks
		if(mod16) ret += mod16 + LS_CRC_SIZE; //possible partial block
	}

	return ret;
}

////////////////////////////////////////////////
//
//	Outgoing frame formatting functions for Sec to Pri transactions
//
////////////////////////////////////////////////

void LinkFrame::FormatAck(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	this->FormatHeader(0, aIsMaster, false, aIsRcvBuffFull, FC_SEC_ACK, aDest, aSrc);
}

void LinkFrame::FormatNack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	this->FormatHeader(0, aIsMaster, false, aIsRcvBuffFull, FC_SEC_NACK, aDest, aSrc);
}

void LinkFrame::FormatLinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	this->FormatHeader(0, aIsMaster, false, aIsRcvBuffFull, FC_SEC_LINK_STATUS, aDest, aSrc);
}

void LinkFrame::FormatNotSupported(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	this->FormatHeader(0, aIsMaster, false, aIsRcvBuffFull, FC_SEC_NOT_SUPPORTED, aDest, aSrc);
}

////////////////////////////////////////////////
//
//	Outgoing frame formatting functions for Pri to Sec transactions
//
////////////////////////////////////////////////

void LinkFrame::FormatResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	this->FormatHeader(0, aIsMaster, false, false, FC_PRI_RESET_LINK_STATES, aDest, aSrc);
}

void LinkFrame::FormatRequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	this->FormatHeader(0, aIsMaster, false, false, FC_PRI_REQUEST_LINK_STATUS, aDest, aSrc);
}

void LinkFrame::FormatTestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc)
{
	this->FormatHeader(0, aIsMaster, aFcb, true, FC_PRI_TEST_LINK_STATES, aDest, aSrc);
}

void LinkFrame::FormatConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength)
{
	assert(aDataLength > 0);
	assert(aDataLength <= 250);
	this->FormatHeader(aDataLength, aIsMaster, aFcb, true, FC_PRI_CONFIRMED_USER_DATA, aDest, aSrc);
	WriteUserData(apData, mpBuffer + LS_HEADER_SIZE, aDataLength);
}

void LinkFrame::FormatUnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength)
{
	assert(aDataLength > 0);
	assert(aDataLength <= 250);
	this->FormatHeader(aDataLength, aIsMaster, false, false, FC_PRI_UNCONFIRMED_USER_DATA, aDest, aSrc);
	WriteUserData(apData, mpBuffer + LS_HEADER_SIZE, aDataLength);
}

void LinkFrame::ChangeFCB(bool aFCB)
{
	if(mHeader.IsFcbSet() != aFCB) {
		mHeader.ChangeFCB(aFCB);
		mHeader.Write(mpBuffer);
	}

}

void LinkFrame::FormatHeader(size_t aDataLength, bool aIsMaster, bool aFcb, bool aFcvDfc, FuncCodes aFuncCode, uint16_t aDest, uint16_t aSrc)
{
	mSize = this->CalcFrameSize(aDataLength);

	mHeader.Set(static_cast<uint8_t>(aDataLength + LS_MIN_LENGTH), aSrc, aDest, aIsMaster, aFcvDfc, aFcb, aFuncCode);
	mHeader.Write(mpBuffer);

	mIsComplete = true;
}

void LinkFrame::WriteUserData(const uint8_t* apSrc, uint8_t* apDest, size_t aLength)
{
	if(aLength == 0) return;
	size_t max = LS_DATA_BLOCK_SIZE;
	size_t num = aLength > max ? max : aLength;
	memcpy(apDest, apSrc, num);
	DNPCrc::AddCrc(apDest, num);
	WriteUserData(apSrc + num, apDest + num + 2, aLength - num); //tail recursive
}

std::string LinkFrame::ToString() const
{
	return mHeader.ToString();
}

}
} //end namespace
