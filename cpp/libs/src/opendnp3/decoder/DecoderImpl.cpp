/*
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

#include "DecoderImpl.h"

#include "opendnp3/app/parsing/APDUHeaderParser.h"
#include "opendnp3/app/parsing/APDUParser.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{
	
DecoderImpl::DecoderImpl(IDecoderCallbacks& callbacks_, openpal::Logger logger_) : 
	callbacks(&callbacks_),
	logger(logger_),
	link(logger_, nullptr),
	transportRx(logger_, 2048, nullptr)
{}

void DecoderImpl::DecodeLPDU(const openpal::RSlice& data)
{
	RSlice remaining(data);

	while (remaining.IsNotEmpty())
	{
		auto dest = this->link.WriteBuff();

		const auto NUM = (remaining.Size() > dest.Size()) ? dest.Size() : remaining.Size();
		
		remaining.Take(NUM).CopyTo(dest);
		link.OnRead(NUM, this);
		
		remaining.Advance(NUM);
	}
	
}

void DecoderImpl::DecodeTPDU(const openpal::RSlice& data)
{
	auto asdu = transportRx.ProcessReceive(data);
	if (asdu.IsNotEmpty())
	{
		this->DecodeAPDU(asdu);
	}
}

void DecoderImpl::DecodeAPDU(const openpal::RSlice& data)
{
	FORMAT_HEX_BLOCK(this->logger, flags::APP_HEX_RX, data, 18, 18);

	if (IsResponse(data))
	{
		APDUResponseHeader header;
		if (APDUHeaderParser::ParseResponse(data, header, &logger))
		{
			FORMAT_LOG_BLOCK(this->logger, flags::APP_HEADER_RX,
	                 "FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s IIN: [0x%02x, 0x%02x]",
	                 header.control.FIR,
	                 header.control.FIN,
	                 header.control.CON,
	                 header.control.UNS,
	                 header.control.SEQ,
	                 FunctionCodeToString(header.function),
	                 header.IIN.LSB,
	                 header.IIN.MSB);

			APDUParser::ParseSinglePass(data.Skip(4), &logger, this, this, ParserSettings::Default());
		}
	}
	else
	{
		APDUHeader header;
		if (APDUHeaderParser::ParseRequest(data, header, &logger))
		{
			
			FORMAT_LOG_BLOCK(this->logger, flags::APP_HEADER_RX,
	                 "FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s",
	                 header.control.FIR,
	                 header.control.FIN,
	                 header.control.CON,
	                 header.control.UNS,
	                 header.control.SEQ,
	                 FunctionCodeToString(header.function));

			auto settings = (header.function == FunctionCode::READ) ? ParserSettings::NoContents() : ParserSettings::Default();
			
			APDUParser::ParseSinglePass(data.Skip(2), &logger, this, this, settings);
		}
	}
}

bool DecoderImpl::IsResponse(const openpal::RSlice& data)
{
	if (data.Size() < 2)
	{
		return false;
	}

	switch (FunctionCodeFromType(data[1]))
	{
		case(FunctionCode::RESPONSE) :
		case(FunctionCode::UNSOLICITED_RESPONSE) :
		case(FunctionCode::AUTH_RESPONSE) :
			return true;
		default:
			return false;
	}
}

bool DecoderImpl::OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata)
{
	if (header.func == LinkFunction::PRI_CONFIRMED_USER_DATA || header.func == LinkFunction::PRI_UNCONFIRMED_USER_DATA)
	{
		this->DecodeTPDU(userdata);		
	}

	return true;
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var1& value, const openpal::RSlice& object) 
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var2& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var5& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var6& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var7& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var10& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var11& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var12& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var13& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var14& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const FreeFormatHeader& header, const Group120Var15& value, const openpal::RSlice& object)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const CountHeader& header, const ICollection<Group50Var1>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const CountHeader& header, const ICollection<Group51Var1>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const CountHeader& header, const ICollection<Group51Var2>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const CountHeader& header, const ICollection<Group52Var1>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const CountHeader& header, const ICollection<Group52Var2>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const CountHeader& header, const ICollection<Group120Var4>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<IINValue>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Binary>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Counter>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Analog>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<OctetString>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<TimeAndInterval>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Group121Var1>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<OctetString>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<TimeAndInterval>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<BinaryCommandEvent>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogCommandEvent>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Group122Var1>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Group122Var2>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<ControlRelayOutputBlock>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt16>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt32>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputFloat32>>& values)
{
	return IINField::Empty();
}

IINField DecoderImpl::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputDouble64>>& values)
{
	return IINField::Empty();
}

}


