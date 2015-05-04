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
#ifndef ASIODNP3_PRINTINGSOEHANDLER_H
#define ASIODNP3_PRINTINGSOEHANDLER_H

#include <opendnp3/master/ISOEHandler.h>

#include <iostream>
#include <sstream>

namespace opendnp3
{

class PrintingSOEHandler : public ISOEHandler
{

public:

	static ISOEHandler& Instance()
	{
		return instance;
	}

	virtual void OnValue(const HeaderInfo& info, const Binary& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const DoubleBitBinary& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const Analog& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const Counter& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const FrozenCounter& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const BinaryOutputStatus& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const AnalogOutputStatus& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const OctetString& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const TimeAndInterval& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const BinaryCommandEvent& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const AnalogCommandEvent& meas, uint16_t index) override final;
	virtual void OnValue(const HeaderInfo& info, const SecurityStat& meas, uint16_t index) override final;
	

protected:

	void Start() final {}
	void End() final {}

private:	

	template <class T>
	static void Print(const HeaderInfo& info, const T& value, uint16_t index)
	{				
		std::cout << "[" << index << "] : " <<
			ValueToString(value) << " : " <<
			static_cast<int>(value.quality) << " : " <<
			value.time << std::endl;		
	}	

	template <class T>
	static std::string ValueToString(const T& meas)
	{
		std::ostringstream oss;
		oss << meas.value;
		return oss.str();
	}

	static std::string GetTimeString(TimestampMode tsmode)
	{
		std::ostringstream oss;	
		switch (tsmode)
		{
			case(TimestampMode::SYNCHRONIZED) :
				return "synchronized";
				break;
			case(TimestampMode::UNSYNCHRONIZED):
				oss << "unsynchronized";
				break;
			default:
				oss << "no timestamp";
				break;
		}
		
		return oss.str();
	}

	static std::string ValueToString(const DoubleBitBinary& meas)
	{
		return DoubleBitToString(meas.value);
	}

	PrintingSOEHandler()
	{}

	static PrintingSOEHandler instance;
};

}

#endif
