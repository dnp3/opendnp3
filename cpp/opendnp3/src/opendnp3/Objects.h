/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __OBJECTS_H_
#define __OBJECTS_H_


#include <opendnp3/ObjectInterfaces.h>
#include <opendnp3/Singleton.h>
#include <opendnp3/QualityMasks.h>
#include <opendnp3/ControlRelayOutputBlock.h>
#include <opendnp3/AnalogOutput.h>


#include "PackingTemplates.h"

#include <openpal/Serialization.h>

#define MACRO_GROUP_VAR_SIZE_FUNC_WITHOUT_EVENTS(group, var, size)\
		MACRO_GROUP_VAR_FUNC(group, var)\
		size_t GetSize() const { return size; }

#define MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(group, var, size) MACRO_GROUP_VAR_SIZE_FUNC_WITHOUT_EVENTS(group, var, size)\
		bool IsEvent() const {return true;}

#define MACRO_GROUP_VAR_SIZE_FUNC(group, var, size) MACRO_GROUP_VAR_SIZE_FUNC_WITHOUT_EVENTS(group, var, size)

#define MACRO_DECLARE_VALUE(packer, position) \
		Pack<packer,position> mValue; \
		typedef packer::Type Type;

// Allows analog types to quickly tell if they're overrange
#define MACRO_DECLARE_VALUE_OVERRANGE(packer, position, qual) \
		PackOverRange<packer,position, qual> mValue; \
		typedef packer::Type Type;

#define MACRO_DECLARE_STREAM_TYPE(datatype) \
		void Write(uint8_t*, const datatype&) const;\
		datatype Read(const uint8_t*) const;

#define MACRO_DECLARE_QUALITY(packer, position) \
		Pack<packer,position> mFlag; \
		bool HasQuality() const { return true; }

#define MACRO_DECLARE_TIME(packer, position) \
		Pack<packer,position> mTime; \
		bool HasTime() const { return true; }


#define MACRO_GROUP_VAR_FUNC(group, var)\
int GetGroup() const { return group; }\
int GetVariation() const { return var; }


namespace opendnp3
{

class Analog;
class Binary;
class Counter;
class SetpointStatus;
class ControlStatus;


///////////////////////////////
//	Binary Input Types
///////////////////////////////

struct Group1Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group1Var0)
	MACRO_GROUP_VAR_FUNC(1, 0)
};

struct Group1Var1:  public BitfieldObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group1Var1)
	MACRO_GROUP_VAR_FUNC(1, 1)
};

struct Group1Var2: public StreamObject<Binary> {
	MACRO_NAME_SINGLETON_INSTANCE(Group1Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(1, 2, 1)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_STREAM_TYPE(Binary)
};

struct Group2Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group2Var0)
	MACRO_GROUP_VAR_FUNC(2, 0)
};

struct Group2Var1 : public StreamObject<Binary> {
	MACRO_NAME_SINGLETON_INSTANCE(Group2Var1)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(2, 1, 1)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_STREAM_TYPE(Binary)
};

struct Group2Var2 : public StreamObject<Binary> {
	MACRO_NAME_SINGLETON_INSTANCE(Group2Var2)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(2, 2, 7)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_TIME(openpal::UInt48, 1)
	MACRO_DECLARE_STREAM_TYPE(Binary)
};

struct Group2Var3 : public StreamObject<Binary> {
	MACRO_NAME_SINGLETON_INSTANCE(Group2Var3)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(2, 3, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_TIME(openpal::UInt16, 1)
	MACRO_DECLARE_STREAM_TYPE(Binary)

	bool UseCTO() const {
		return true;
	}
};

///////////////////////////////
//	Binary Output Types
///////////////////////////////

struct Group10Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group10Var0)
	MACRO_GROUP_VAR_FUNC(10, 0)
};

struct Group10Var1 : public BitfieldObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group10Var1)
	MACRO_GROUP_VAR_FUNC(10, 1)
};

struct Group10Var2 : public StreamObject<ControlStatus> {
	MACRO_NAME_SINGLETON_INSTANCE(Group10Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(10, 2, 1)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_STREAM_TYPE(ControlStatus);
};

///////////////////////////////
//	Control Block Types
///////////////////////////////

struct Group12Var1 : public CommandObject<ControlRelayOutputBlock> {
	MACRO_NAME_SINGLETON_INSTANCE(Group12Var1)
	MACRO_GROUP_VAR_SIZE_FUNC(12, 1, 11)
	MACRO_DECLARE_STREAM_TYPE(ControlRelayOutputBlock);

	Pack<openpal::UInt8, 0>		mCode;
	Pack<openpal::UInt8, 1>		mCount;
	Pack<openpal::UInt32, 2>	mOnTime;
	Pack<openpal::UInt32, 6>	mOffTime;
	Pack<openpal::UInt8, 10>		mStatus;

	CopyableBuffer GetValueBytes(const uint8_t*) const;
};

struct Group12Var2 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group12Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(12, 2, 11)

	Pack<openpal::UInt8, 0>		mCode;
	Pack<openpal::UInt8, 1>		mCount;
	Pack<openpal::UInt32, 2>	mOnTime;
	Pack<openpal::UInt32, 6>	mOffTime;
	Pack<openpal::UInt8, 10>		mStatus;
};

struct Group12Var3 : public BitfieldObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group12Var3)
	MACRO_GROUP_VAR_FUNC(12, 3)
};

///////////////////////////////
//	Static Counter Types
///////////////////////////////

struct Group20Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group20Var0)
	MACRO_GROUP_VAR_FUNC(20, 0)
};

struct Group20Var1 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group20Var1)
	MACRO_GROUP_VAR_SIZE_FUNC(20, 1, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0);
	MACRO_DECLARE_VALUE(openpal::UInt32, 1);
	MACRO_DECLARE_STREAM_TYPE(Counter);

};

struct Group20Var2 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group20Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(20, 2, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0);
	MACRO_DECLARE_VALUE(openpal::UInt16, 1);
	MACRO_DECLARE_STREAM_TYPE(Counter);
};

struct Group20Var3 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group20Var3)
	MACRO_GROUP_VAR_SIZE_FUNC(20, 3, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0);
	MACRO_DECLARE_VALUE(openpal::UInt32, 1);
	MACRO_DECLARE_STREAM_TYPE(Counter);
};

struct Group20Var4 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group20Var4)
	MACRO_GROUP_VAR_SIZE_FUNC(20, 4, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0);
	MACRO_DECLARE_VALUE(openpal::UInt16, 1);
	MACRO_DECLARE_STREAM_TYPE(Counter);

};

struct Group20Var5 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group20Var5)
	MACRO_GROUP_VAR_SIZE_FUNC(20, 5, 4)
	MACRO_DECLARE_VALUE(openpal::UInt32, 0);
	MACRO_DECLARE_STREAM_TYPE(Counter);
};

struct Group20Var6 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group20Var6)
	MACRO_GROUP_VAR_SIZE_FUNC(20, 6, 2)
	MACRO_DECLARE_VALUE(openpal::UInt16, 0);
	MACRO_DECLARE_STREAM_TYPE(Counter);
};

struct Group20Var7 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group20Var7)
	MACRO_GROUP_VAR_SIZE_FUNC(20, 7, 4)
	MACRO_DECLARE_VALUE(openpal::UInt32, 0);
	MACRO_DECLARE_STREAM_TYPE(Counter);
};

struct Group20Var8 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group20Var8)
	MACRO_GROUP_VAR_SIZE_FUNC(20, 8, 2)
	MACRO_DECLARE_VALUE(openpal::UInt16, 0)
	MACRO_DECLARE_STREAM_TYPE(Counter)
};

///////////////////////////////
//	Frozen Counter Types
///////////////////////////////

struct Group21Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var0)
	MACRO_GROUP_VAR_FUNC(21, 0)
};

struct Group21Var1 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var1)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 1, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
};

struct Group21Var2 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 2, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
};

struct Group21Var3 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var3)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 3, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
};

struct Group21Var4 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var4)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 4, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
};

struct Group21Var5 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var5)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 5, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
};

struct Group21Var6 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var6)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 6, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 3)
};

struct Group21Var7 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var7)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 7, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
};

struct Group21Var8 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var8)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 8, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 3)
};

struct Group21Var9 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var9)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 9, 4)
	MACRO_DECLARE_VALUE(openpal::UInt32, 0)
};

struct Group21Var10 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var10)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 10, 2)
	MACRO_DECLARE_VALUE(openpal::UInt16, 0)
};

struct Group21Var11 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var11)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 11, 4)
	MACRO_DECLARE_VALUE(openpal::UInt32, 0)
};

struct Group21Var12 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group21Var12)
	MACRO_GROUP_VAR_SIZE_FUNC(21, 12, 2)
	MACRO_DECLARE_VALUE(openpal::UInt16, 0)
};


///////////////////////////////
// Counter Event Types
///////////////////////////////

struct Group22Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group22Var0)
	MACRO_GROUP_VAR_FUNC(22, 0)
};

struct Group22Var1 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group22Var1)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(22, 1, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
	MACRO_DECLARE_STREAM_TYPE(Counter)
};

struct Group22Var2 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group22Var2)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(22, 2, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
	MACRO_DECLARE_STREAM_TYPE(Counter)
};

struct Group22Var3 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group22Var3)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(22, 3, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
	MACRO_DECLARE_STREAM_TYPE(Counter)
};

struct Group22Var4 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group22Var4)
	MACRO_GROUP_VAR_SIZE_FUNC(22, 4, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
	MACRO_DECLARE_STREAM_TYPE(Counter)
};

struct Group22Var5 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group22Var5)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(22, 5, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
	MACRO_DECLARE_STREAM_TYPE(Counter)
};

struct Group22Var6 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group22Var6)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(22, 6, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 3)
	MACRO_DECLARE_STREAM_TYPE(Counter)
};

struct Group22Var7 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group22Var7)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(22, 7, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
	MACRO_DECLARE_STREAM_TYPE(Counter)
};

struct Group22Var8 : public StreamObject<Counter> {
	MACRO_NAME_SINGLETON_INSTANCE(Group22Var8)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(22, 8, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 3)
	MACRO_DECLARE_STREAM_TYPE(Counter)
};

///////////////////////////////
// Frozen Counter Event Types
///////////////////////////////

struct Group23Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group23Var0)
	MACRO_GROUP_VAR_FUNC(23, 0)
};

struct Group23Var1 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group23Var1)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(23, 1, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
};

struct Group23Var2 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group23Var2)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(23, 2, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
};

struct Group23Var3 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group23Var3)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(23, 3, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
};

struct Group23Var4 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group23Var4)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(23, 4, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
};

struct Group23Var5 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group23Var5)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(23, 5, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
};

struct Group23Var6 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group23Var6)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(23, 6, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 3)
};

struct Group23Var7 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group23Var7)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(23, 7, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt32, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
};

struct Group23Var8 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group23Var8)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(23, 8, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::UInt16, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 3)
};

///////////////////////////////
//	Analog Input Types
///////////////////////////////

struct Group30Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group30Var0)
	MACRO_GROUP_VAR_FUNC(30, 0)
};

struct Group30Var1 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group30Var1)
	MACRO_GROUP_VAR_SIZE_FUNC(30, 1, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int32, 1, AQ_OVERRANGE)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group30Var2 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group30Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(30, 2, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int16, 1, AQ_OVERRANGE)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group30Var3 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group30Var3)
	MACRO_GROUP_VAR_SIZE_FUNC(30, 3, 4)
	MACRO_DECLARE_VALUE(openpal::Int32, 0)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group30Var4 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group30Var4)
	MACRO_GROUP_VAR_SIZE_FUNC(30, 4, 2)
	MACRO_DECLARE_VALUE(openpal::Int16, 0)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group30Var5 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group30Var5)
	MACRO_GROUP_VAR_SIZE_FUNC(30, 5, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::SingleFloat, 1, AQ_OVERRANGE)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group30Var6 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group30Var6)
	MACRO_GROUP_VAR_SIZE_FUNC(30, 6, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::DoubleFloat, 1, AQ_OVERRANGE)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

///////////////////////////////
//	Frozen Analog Input Types
///////////////////////////////

struct Group31Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group31Var0)
	MACRO_GROUP_VAR_FUNC(31, 0)
};

struct Group31Var1 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group31Var1)
	MACRO_GROUP_VAR_SIZE_FUNC(31, 1, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int32, 1, AQ_OVERRANGE)
};

struct Group31Var2 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group31Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(31, 2, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int16, 1, AQ_OVERRANGE)
};

struct Group31Var3 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group31Var3)
	MACRO_GROUP_VAR_SIZE_FUNC(31, 3, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int32, 1, AQ_OVERRANGE)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
};

struct Group31Var4 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group31Var4)
	MACRO_GROUP_VAR_SIZE_FUNC(31, 4, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int16, 1, AQ_OVERRANGE)
	MACRO_DECLARE_TIME(openpal::UInt48, 3)
};

struct Group31Var5 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group31Var5)
	MACRO_GROUP_VAR_SIZE_FUNC(31, 5, 4)
	MACRO_DECLARE_VALUE(openpal::Int32, 0)
};

struct Group31Var6 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group31Var6)
	MACRO_GROUP_VAR_SIZE_FUNC(31, 6, 2)
	MACRO_DECLARE_VALUE(openpal::Int16, 0)
};

struct Group31Var7 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group31Var7)
	MACRO_GROUP_VAR_SIZE_FUNC(31, 7, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::SingleFloat, 1)
};

struct Group31Var8 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group31Var8)
	MACRO_GROUP_VAR_SIZE_FUNC(31, 8, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::DoubleFloat, 1)
};

///////////////////////////////
//	Analog Change Event Types
///////////////////////////////

struct Group32Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group32Var0)
	MACRO_GROUP_VAR_FUNC(32, 0)
};

struct Group32Var1 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group32Var1)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(32, 1, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int32, 1, AQ_OVERRANGE)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group32Var2 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group32Var2)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(32, 2, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int16, 1, AQ_OVERRANGE)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group32Var3 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group32Var3)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(32, 3, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int32, 1, AQ_OVERRANGE)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group32Var4 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group32Var4)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(32, 4, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int16, 1, AQ_OVERRANGE)
	MACRO_DECLARE_TIME(openpal::UInt48, 3)
	MACRO_DECLARE_STREAM_TYPE(Analog);
};

struct Group32Var5 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group32Var5)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(32, 5, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::SingleFloat, 1)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group32Var6 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group32Var6)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(32, 6, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::DoubleFloat, 1)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group32Var7 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group32Var7)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(32, 7, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::SingleFloat, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

struct Group32Var8 : public StreamObject<Analog> {
	MACRO_NAME_SINGLETON_INSTANCE(Group32Var8)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(32, 8, 15)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::DoubleFloat, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 9)
	MACRO_DECLARE_STREAM_TYPE(Analog)
};

///////////////////////////////
//	Frozen Analog Change Event Types
///////////////////////////////

struct Group33Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group33Var0)
	MACRO_GROUP_VAR_FUNC(33, 0)
};

struct Group33Var1 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group33Var1)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(33, 1, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int32, 1, AQ_OVERRANGE)
};

struct Group33Var2 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group33Var2)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(33, 2, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int16, 1, AQ_OVERRANGE)
};

struct Group33Var3 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group33Var3)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(33, 3, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int32, 1, AQ_OVERRANGE)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
};

struct Group33Var4 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group33Var4)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(33, 4, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE_OVERRANGE(openpal::Int16, 1, AQ_OVERRANGE)
	MACRO_DECLARE_TIME(openpal::UInt48, 3)
};

struct Group33Var5 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group33Var5)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(33, 5, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::SingleFloat, 1)
};

struct Group33Var6 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group33Var6)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(33, 6, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::DoubleFloat, 1)
};

struct Group33Var7 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group33Var7)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(33, 7, 11)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::SingleFloat, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 5)
};

struct Group33Var8 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group33Var8)
	MACRO_GROUP_VAR_SIZE_FUNC_WITH_EVENTS(33, 8, 15)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::DoubleFloat, 1)
	MACRO_DECLARE_TIME(openpal::UInt48, 9)
};



///////////////////////////////
//	Analog Output Types
///////////////////////////////

struct Group40Var0 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group40Var0)
	MACRO_GROUP_VAR_FUNC(40, 0)
};

struct Group40Var1 : public StreamObject<SetpointStatus> {
	MACRO_NAME_SINGLETON_INSTANCE(Group40Var1)
	MACRO_GROUP_VAR_SIZE_FUNC(40, 1, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::Int32, 1)
	MACRO_DECLARE_STREAM_TYPE(SetpointStatus)
};

struct Group40Var2 : public StreamObject<SetpointStatus> {
	MACRO_NAME_SINGLETON_INSTANCE(Group40Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(40, 2, 3)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::Int16, 1)
	MACRO_DECLARE_STREAM_TYPE(SetpointStatus)
};

struct Group40Var3 : public StreamObject<SetpointStatus> {
	MACRO_NAME_SINGLETON_INSTANCE(Group40Var3)
	MACRO_GROUP_VAR_SIZE_FUNC(40, 3, 5)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::SingleFloat, 1)
	MACRO_DECLARE_STREAM_TYPE(SetpointStatus)
};

struct Group40Var4 : public StreamObject<SetpointStatus> {
	MACRO_NAME_SINGLETON_INSTANCE(Group40Var4)
	MACRO_GROUP_VAR_SIZE_FUNC(40, 4, 9)
	MACRO_DECLARE_QUALITY(openpal::UInt8, 0)
	MACRO_DECLARE_VALUE(openpal::DoubleFloat, 1)
	MACRO_DECLARE_STREAM_TYPE(SetpointStatus)
};

struct Group41Var1 : public CommandObject<AnalogOutputInt32> {
	MACRO_NAME_SINGLETON_INSTANCE(Group41Var1)
	MACRO_GROUP_VAR_SIZE_FUNC(41, 1, 5)
	MACRO_DECLARE_VALUE(openpal::Int32, 0)
	Pack<openpal::UInt8, 4> mStatus;
	MACRO_DECLARE_STREAM_TYPE(AnalogOutputInt32)

	CopyableBuffer GetValueBytes(const uint8_t*) const;
};

struct Group41Var2 : public CommandObject<AnalogOutputInt16> {
	MACRO_NAME_SINGLETON_INSTANCE(Group41Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(41, 2, 3)
	MACRO_DECLARE_VALUE(openpal::Int16, 0)
	Pack<openpal::UInt8, 2>		mStatus;
	MACRO_DECLARE_STREAM_TYPE(AnalogOutputInt16)

	CopyableBuffer GetValueBytes(const uint8_t*) const;
};

struct Group41Var3 : public CommandObject<AnalogOutputFloat32> {
	MACRO_NAME_SINGLETON_INSTANCE(Group41Var3)
	MACRO_GROUP_VAR_SIZE_FUNC(41, 3, 5)
	MACRO_DECLARE_VALUE(openpal::SingleFloat, 0)
	Pack<openpal::UInt8, 4>		mStatus;
	MACRO_DECLARE_STREAM_TYPE(AnalogOutputFloat32)

	CopyableBuffer GetValueBytes(const uint8_t*) const;
};

struct Group41Var4 : public CommandObject<AnalogOutputDouble64> {
	MACRO_NAME_SINGLETON_INSTANCE(Group41Var4)
	MACRO_GROUP_VAR_SIZE_FUNC(41, 4, 9)
	MACRO_DECLARE_VALUE(openpal::DoubleFloat, 0)
	Pack<openpal::UInt8, 8>		mStatus;
	MACRO_DECLARE_STREAM_TYPE(AnalogOutputDouble64)

	CopyableBuffer GetValueBytes(const uint8_t*) const;
};

///////////////////////////////
//	Time Types
///////////////////////////////

struct Group50Var1 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group50Var1)
	MACRO_GROUP_VAR_SIZE_FUNC(50, 1, 6)
	MACRO_DECLARE_TIME(openpal::UInt48, 0)
};

struct Group50Var2 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group50Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(50, 2, 10)
	MACRO_DECLARE_TIME(openpal::UInt48, 0)
	Pack<openpal::UInt32, 0>	mInterval;
};

struct Group51Var1 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group51Var1)
	MACRO_GROUP_VAR_SIZE_FUNC(51, 1, 6)
	MACRO_DECLARE_TIME(openpal::UInt48, 0)
};

struct Group51Var2 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group51Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(51, 2, 6)
	MACRO_DECLARE_TIME(openpal::UInt48, 0)
};

struct Group52Var1 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group52Var1)
	MACRO_GROUP_VAR_SIZE_FUNC(52, 1, 2)
	MACRO_DECLARE_TIME(openpal::UInt16, 0)
};

struct Group52Var2 : public FixedObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group52Var2)
	MACRO_GROUP_VAR_SIZE_FUNC(52, 2, 2)
	MACRO_DECLARE_TIME(openpal::UInt16, 0)
};

///////////////////////////////
//	Class Types
///////////////////////////////

struct Group60Var1 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group60Var1)
	MACRO_GROUP_VAR_FUNC(60, 1)
};

struct Group60Var2 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group60Var2)
	MACRO_GROUP_VAR_FUNC(60, 2)
};

struct Group60Var3 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group60Var3)
	MACRO_GROUP_VAR_FUNC(60, 3)
};

struct Group60Var4 : public PlaceHolderObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group60Var4)
	MACRO_GROUP_VAR_FUNC(60, 4)
};

///////////////////////////////
//	Device Object Types
///////////////////////////////

struct Group80Var1 : public BitfieldObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group80Var1)
	MACRO_GROUP_VAR_FUNC(80, 1)
};

struct Group110Var0 : public SizeByVariationObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group110Var0)
	MACRO_GROUP_VAR_FUNC(110, 0)
};

struct Group111Var0 : public SizeByVariationObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group111Var0)
	MACRO_GROUP_VAR_FUNC(111, 0)
};

struct Group112Var0 : public SizeByVariationObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group112Var0)
	MACRO_GROUP_VAR_FUNC(112, 0)
};

struct Group113Var0 : public SizeByVariationObject {
	MACRO_NAME_SINGLETON_INSTANCE(Group113Var0)
	MACRO_GROUP_VAR_FUNC(113, 0)
};

}

#endif

