
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
#include "Objects.h"

#include <APL/DataTypes.h>
#include <APL/CommandTypes.h>
#include <APL/CopyableBuffer.h>

#include "DNPToStream.h"
#include "DNPFromStream.h"

#define MACRO_STATIC_INSTANCE(group, var) Group##group##Var##var Group##group##Var##var::mInstance;

namespace apl
{
namespace dnp
{

MACRO_STATIC_INSTANCE(1, 0)
MACRO_STATIC_INSTANCE(1, 1)
MACRO_STATIC_INSTANCE(1, 2)
MACRO_STATIC_INSTANCE(2, 0)
MACRO_STATIC_INSTANCE(2, 1)
MACRO_STATIC_INSTANCE(2, 2)
MACRO_STATIC_INSTANCE(2, 3)

MACRO_STATIC_INSTANCE(10, 0)
MACRO_STATIC_INSTANCE(10, 1)
MACRO_STATIC_INSTANCE(10, 2)

MACRO_STATIC_INSTANCE(12, 1)
MACRO_STATIC_INSTANCE(12, 2)
MACRO_STATIC_INSTANCE(12, 3)


MACRO_STATIC_INSTANCE(20, 0)
MACRO_STATIC_INSTANCE(20, 1)
MACRO_STATIC_INSTANCE(20, 2)
MACRO_STATIC_INSTANCE(20, 3)
MACRO_STATIC_INSTANCE(20, 4)
MACRO_STATIC_INSTANCE(20, 5)
MACRO_STATIC_INSTANCE(20, 6)
MACRO_STATIC_INSTANCE(20, 7)
MACRO_STATIC_INSTANCE(20, 8)

MACRO_STATIC_INSTANCE(21, 0)
MACRO_STATIC_INSTANCE(21, 1)
MACRO_STATIC_INSTANCE(21, 2)
MACRO_STATIC_INSTANCE(21, 3)
MACRO_STATIC_INSTANCE(21, 4)
MACRO_STATIC_INSTANCE(21, 5)
MACRO_STATIC_INSTANCE(21, 6)
MACRO_STATIC_INSTANCE(21, 7)
MACRO_STATIC_INSTANCE(21, 8)
MACRO_STATIC_INSTANCE(21, 9)
MACRO_STATIC_INSTANCE(21, 10)
MACRO_STATIC_INSTANCE(21, 11)
MACRO_STATIC_INSTANCE(21, 12)

MACRO_STATIC_INSTANCE(22, 0)
MACRO_STATIC_INSTANCE(22, 1)
MACRO_STATIC_INSTANCE(22, 2)
MACRO_STATIC_INSTANCE(22, 3)
MACRO_STATIC_INSTANCE(22, 4)
MACRO_STATIC_INSTANCE(22, 5)
MACRO_STATIC_INSTANCE(22, 6)
MACRO_STATIC_INSTANCE(22, 7)
MACRO_STATIC_INSTANCE(22, 8)

MACRO_STATIC_INSTANCE(23, 0)
MACRO_STATIC_INSTANCE(23, 1)
MACRO_STATIC_INSTANCE(23, 2)
MACRO_STATIC_INSTANCE(23, 3)
MACRO_STATIC_INSTANCE(23, 4)
MACRO_STATIC_INSTANCE(23, 5)
MACRO_STATIC_INSTANCE(23, 6)
MACRO_STATIC_INSTANCE(23, 7)
MACRO_STATIC_INSTANCE(23, 8)

MACRO_STATIC_INSTANCE(30, 0)
MACRO_STATIC_INSTANCE(30, 1)
MACRO_STATIC_INSTANCE(30, 2)
MACRO_STATIC_INSTANCE(30, 3)
MACRO_STATIC_INSTANCE(30, 4)
MACRO_STATIC_INSTANCE(30, 5)
MACRO_STATIC_INSTANCE(30, 6)

MACRO_STATIC_INSTANCE(31, 0)
MACRO_STATIC_INSTANCE(31, 1)
MACRO_STATIC_INSTANCE(31, 2)
MACRO_STATIC_INSTANCE(31, 3)
MACRO_STATIC_INSTANCE(31, 4)
MACRO_STATIC_INSTANCE(31, 5)
MACRO_STATIC_INSTANCE(31, 6)

MACRO_STATIC_INSTANCE(32, 0)
MACRO_STATIC_INSTANCE(32, 1)
MACRO_STATIC_INSTANCE(32, 2)
MACRO_STATIC_INSTANCE(32, 3)
MACRO_STATIC_INSTANCE(32, 4)
MACRO_STATIC_INSTANCE(32, 5)
MACRO_STATIC_INSTANCE(32, 6)
MACRO_STATIC_INSTANCE(32, 7)
MACRO_STATIC_INSTANCE(32, 8)

MACRO_STATIC_INSTANCE(33, 0)
MACRO_STATIC_INSTANCE(33, 1)
MACRO_STATIC_INSTANCE(33, 2)
MACRO_STATIC_INSTANCE(33, 3)
MACRO_STATIC_INSTANCE(33, 4)
MACRO_STATIC_INSTANCE(33, 5)
MACRO_STATIC_INSTANCE(33, 6)
MACRO_STATIC_INSTANCE(33, 7)
MACRO_STATIC_INSTANCE(33, 8)

MACRO_STATIC_INSTANCE(40, 0)
MACRO_STATIC_INSTANCE(40, 1)
MACRO_STATIC_INSTANCE(40, 2)
MACRO_STATIC_INSTANCE(40, 3)
MACRO_STATIC_INSTANCE(40, 4)

MACRO_STATIC_INSTANCE(41, 1)
MACRO_STATIC_INSTANCE(41, 2)
MACRO_STATIC_INSTANCE(41, 3)
MACRO_STATIC_INSTANCE(41, 4)

MACRO_STATIC_INSTANCE(50, 1)
MACRO_STATIC_INSTANCE(50, 2)
MACRO_STATIC_INSTANCE(51, 1)
MACRO_STATIC_INSTANCE(51, 2)
MACRO_STATIC_INSTANCE(52, 1)
MACRO_STATIC_INSTANCE(52, 2)

MACRO_STATIC_INSTANCE(60, 1)
MACRO_STATIC_INSTANCE(60, 2)
MACRO_STATIC_INSTANCE(60, 3)
MACRO_STATIC_INSTANCE(60, 4)


MACRO_STATIC_INSTANCE(80, 1)

MACRO_STATIC_INSTANCE(110, 0)
MACRO_STATIC_INSTANCE(111, 0)
MACRO_STATIC_INSTANCE(112, 0)
MACRO_STATIC_INSTANCE(113, 0)

///////////////////////////////
//	Binary Input Types
///////////////////////////////

void Group1Var2::Write(uint8_t* p, const apl::Binary& v) const
{
	DNPToStream::WriteQ(p, Group1Var2::Inst(), v);
}
void Group2Var1::Write(uint8_t* p, const apl::Binary& v) const
{
	DNPToStream::WriteQ(p, Group2Var1::Inst(), v);
}
void Group2Var2::Write(uint8_t* p, const apl::Binary& v) const
{
	DNPToStream::WriteQT(p, Group2Var2::Inst(), v);
}
void Group2Var3::Write(uint8_t* p, const apl::Binary& v) const
{
	DNPToStream::WriteQT(p, Group2Var3::Inst(), v);
}

Binary Group1Var2::Read(const uint8_t* p) const
{
	return DNPFromStream::ReadBinaryQV(p, Group1Var2::Inst());
}
Binary Group2Var1::Read(const uint8_t* p) const
{
	return DNPFromStream::ReadBinaryQV(p, Group2Var1::Inst());
}
Binary Group2Var2::Read(const uint8_t* p) const
{
	return DNPFromStream::ReadBinaryQV(p, Group2Var2::Inst());
}
Binary Group2Var3::Read(const uint8_t* p) const
{
	return DNPFromStream::ReadBinaryQVT(p, Group2Var3::Inst());
}


///////////////////////////////
//	Binary Output Status
///////////////////////////////

ControlStatus Group10Var2::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQ(apPos, Group10Var2::Inst());
}
void Group10Var2::Write(uint8_t* apPos, const ControlStatus& arObj) const
{
	DNPToStream::WriteQ(apPos, Group10Var2::Inst(), arObj);
}

///////////////////////////////
//	Binary Output Types
///////////////////////////////

void Group12Var1::Write(uint8_t* apPos, const BinaryOutput& arControl) const
{
	mCode.Set(apPos, arControl.mRawCode);
	mCount.Set(apPos, arControl.mCount);
	mOffTime.Set(apPos, arControl.mOffTimeMS);
	mOnTime.Set(apPos, arControl.mOnTimeMS);
	mStatus.Set(apPos, arControl.mStatus);
}

BinaryOutput Group12Var1::Read(const uint8_t* apPos) const
{
	BinaryOutput b;
	b.mRawCode = mCode.Get(apPos);
	b.mCount = mCount.Get(apPos);
	b.mOffTimeMS = mOffTime.Get(apPos);
	b.mOnTimeMS = mOnTime.Get(apPos);
	b.mStatus = ByteToCommandStatus(mStatus.Get(apPos));
	return b;
}

apl::CopyableBuffer Group12Var1::GetValueBytes(const uint8_t* apPos) const
{
	return CopyableBuffer(apPos, 10); //first 10 bytes, everything but the status
}

///////////////////////////////
// Counter Types
///////////////////////////////

Counter Group20Var1::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group20Var1::Inst());
}
Counter Group20Var2::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group20Var2::Inst());
}
Counter Group20Var3::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group20Var3::Inst());
}
Counter Group20Var4::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group20Var4::Inst());
}
Counter Group20Var5::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadV(apPos, Group20Var5::Inst());
}
Counter Group20Var6::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadV(apPos, Group20Var6::Inst());
}
Counter Group20Var7::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadV(apPos, Group20Var7::Inst());
}
Counter Group20Var8::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadV(apPos, Group20Var8::Inst());
}

void Group20Var1::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQV(apPos, Group20Var1::Inst(), v);
}
void Group20Var2::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQV(apPos, Group20Var2::Inst(), v);
}
void Group20Var3::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQV(apPos, Group20Var3::Inst(), v);
}
void Group20Var4::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQV(apPos, Group20Var4::Inst(), v);
}
void Group20Var5::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteV(apPos, Group20Var5::Inst(), v);
}
void Group20Var6::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteV(apPos, Group20Var6::Inst(), v);
}
void Group20Var7::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteV(apPos, Group20Var7::Inst(), v);
}
void Group20Var8::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteV(apPos, Group20Var8::Inst(), v);
}

Counter Group22Var1::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group22Var1::Inst());
}
Counter Group22Var2::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group22Var2::Inst());
}
Counter Group22Var3::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group22Var3::Inst());
}
Counter Group22Var4::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group22Var4::Inst());
}
Counter Group22Var5::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQVT(apPos, Group22Var5::Inst());
}
Counter Group22Var6::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQVT(apPos, Group22Var6::Inst());
}
Counter Group22Var7::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQVT(apPos, Group22Var7::Inst());
}
Counter Group22Var8::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQVT(apPos, Group22Var8::Inst());
}

void Group22Var1::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQV(apPos, Group22Var1::Inst(), v);
}
void Group22Var2::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQV(apPos, Group22Var2::Inst(), v);
}
void Group22Var3::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQV(apPos, Group22Var3::Inst(), v);
}
void Group22Var4::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQV(apPos, Group22Var4::Inst(), v);
}
void Group22Var5::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQVT(apPos, Group22Var5::Inst(), v);
}
void Group22Var6::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQVT(apPos, Group22Var6::Inst(), v);
}
void Group22Var7::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQVT(apPos, Group22Var7::Inst(), v);
}
void Group22Var8::Write(uint8_t* apPos, const apl::Counter& v) const
{
	DNPToStream::WriteQVT(apPos, Group22Var8::Inst(), v);
}


///////////////////////////////
//	Analog Input Types
///////////////////////////////
void Group30Var1::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteCheckRangeQV(p, Group30Var1::Inst(), v);
}
void Group30Var2::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteCheckRangeQV(p, Group30Var2::Inst(), v);
}
void Group30Var3::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteV(p, Group30Var3::Inst(), v);
}
void Group30Var4::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteV(p, Group30Var4::Inst(), v);
}
void Group30Var5::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteCheckRangeQV(p, Group30Var5::Inst(), v);
}
void Group30Var6::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteCheckRangeQV(p, Group30Var6::Inst(), v);
}

Analog Group30Var1::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group30Var1::Inst());
}
Analog Group30Var2::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group30Var2::Inst());
}
Analog Group30Var3::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadV(apPos, Group30Var3::Inst());
}
Analog Group30Var4::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadV(apPos, Group30Var4::Inst());
}
Analog Group30Var5::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group30Var5::Inst());
}
Analog Group30Var6::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group30Var6::Inst());
}

void Group32Var1::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteCheckRangeQV(p, Group32Var1::Inst(), v);
}
void Group32Var2::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteCheckRangeQV(p, Group32Var2::Inst(), v);
}
void Group32Var3::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteCheckRangeQVT(p, Group32Var3::Inst(), v);
}
void Group32Var4::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteCheckRangeQVT(p, Group32Var4::Inst(), v);
}
void Group32Var5::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteQV(p, Group32Var5::Inst(), v);
}
void Group32Var6::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteQV(p, Group32Var6::Inst(), v);
}
void Group32Var7::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteQVT(p, Group32Var7::Inst(), v);
}
void Group32Var8::Write(uint8_t* p, const apl::Analog& v) const
{
	DNPToStream::WriteQVT(p, Group32Var8::Inst(), v);
}

Analog Group32Var1::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group32Var1::Inst());
}
Analog Group32Var2::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group32Var2::Inst());
}
Analog Group32Var3::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQVT(apPos, Group32Var3::Inst());
}
Analog Group32Var4::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQVT(apPos, Group32Var4::Inst());
}
Analog Group32Var5::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group32Var5::Inst());
}
Analog Group32Var6::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group32Var6::Inst());
}
Analog Group32Var7::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQVT(apPos, Group32Var7::Inst());
}
Analog Group32Var8::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQVT(apPos, Group32Var8::Inst());
}

///////////////////////////////
//	Analog Output Status
///////////////////////////////

SetpointStatus Group40Var1::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group40Var1::Inst());
}
SetpointStatus Group40Var2::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group40Var2::Inst());
}
SetpointStatus Group40Var3::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group40Var3::Inst());
}
SetpointStatus Group40Var4::Read(const uint8_t* apPos) const
{
	return DNPFromStream::ReadQV(apPos, Group40Var4::Inst());
}

void Group40Var1::Write(uint8_t* apPos, const apl::SetpointStatus& arObj) const
{
	DNPToStream::WriteQV(apPos, Group40Var1::Inst(), arObj);
}
void Group40Var2::Write(uint8_t* apPos, const apl::SetpointStatus& arObj) const
{
	DNPToStream::WriteQV(apPos, Group40Var2::Inst(), arObj);
}
void Group40Var3::Write(uint8_t* apPos, const apl::SetpointStatus& arObj) const
{
	DNPToStream::WriteQV(apPos, Group40Var3::Inst(), arObj);
}
void Group40Var4::Write(uint8_t* apPos, const apl::SetpointStatus& arObj) const
{
	DNPToStream::WriteQV(apPos, Group40Var4::Inst(), arObj);
}

///////////////////////////////
//	Setpoint Types
///////////////////////////////

void Group41Var1::Write(uint8_t* apBuff, const Setpoint& arVal) const
{
	this->mValue.Set(apBuff, arVal.GetIntValue());
	this->mStatus.Set(apBuff, arVal.mStatus);
}

Setpoint Group41Var1::Read(const uint8_t* apBuff) const
{
	Setpoint s(static_cast<int32_t>(this->mValue.Get(apBuff)));
	s.mStatus = ByteToCommandStatus(this->mStatus.Get(apBuff));
	s.SetEncodingType(SPET_INT32);
	return s;
}

apl::CopyableBuffer Group41Var1::GetValueBytes(const uint8_t* apBuff) const
{
	return CopyableBuffer(apBuff, 4);
}

void Group41Var2::Write(uint8_t* apBuff, const Setpoint& arVal) const
{
	this->mValue.Set(apBuff, static_cast<uint16_t>(arVal.GetIntValue()));
	this->mStatus.Set(apBuff, arVal.mStatus);
}

Setpoint Group41Var2::Read(const uint8_t* apBuff) const
{
	Setpoint s(static_cast<int16_t>(this->mValue.Get(apBuff)));
	s.mStatus = ByteToCommandStatus(this->mStatus.Get(apBuff));
	s.SetEncodingType(SPET_INT16);
	return s;
}

apl::CopyableBuffer Group41Var2::GetValueBytes(const uint8_t* apBuff) const
{
	return CopyableBuffer(apBuff, 2);
}

void Group41Var3::Write(uint8_t* apBuff, const Setpoint& arVal) const
{
	this->mValue.Set(apBuff, static_cast<float>(arVal.GetValue()));
	this->mStatus.Set(apBuff, arVal.mStatus);
}

Setpoint Group41Var3::Read(const uint8_t* apBuff) const
{
	Setpoint s(this->mValue.Get(apBuff));
	s.mStatus = ByteToCommandStatus(this->mStatus.Get(apBuff));
	s.SetEncodingType(SPET_FLOAT);
	return s;
}

apl::CopyableBuffer Group41Var3::GetValueBytes(const uint8_t* apBuff) const
{
	return CopyableBuffer(apBuff, 4);
}

void Group41Var4::Write(uint8_t* apBuff, const Setpoint& arVal) const
{
	this->mValue.Set(apBuff, arVal.GetValue());
	this->mStatus.Set(apBuff, arVal.mStatus);
}

Setpoint Group41Var4::Read(const uint8_t* apBuff) const
{
	Setpoint s(this->mValue.Get(apBuff));
	s.mStatus = ByteToCommandStatus(this->mStatus.Get(apBuff));
	s.SetEncodingType(SPET_DOUBLE);
	return s;
}

apl::CopyableBuffer Group41Var4::GetValueBytes(const uint8_t* apBuff) const
{
	return CopyableBuffer(apBuff, 8);
}
}
}
