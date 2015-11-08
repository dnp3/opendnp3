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
#ifndef OPENDNP3_DECODERIMPL_H
#define OPENDNP3_DECODERIMPL_H

#include <openpal/container/RSlice.h>
#include <openpal/logging/Logger.h>

#include <opendnp3/decoder/IDecoderCallbacks.h>

#include "opendnp3/link/LinkLayerParser.h"
#include "opendnp3/link/IFrameSink.h"
#include "opendnp3/transport/TransportRx.h"
#include "opendnp3/app/parsing/IAPDUHandler.h"

namespace opendnp3
{
	class DecoderImpl;

	// stand-alone DNP3 decoder
	class DecoderImpl final : private IFrameSink, private IAPDUHandler
	{
	public:

		DecoderImpl(IDecoderCallbacks& callbacks, openpal::Logger logger);		

		void DecodeLPDU(const openpal::RSlice& data);
		void DecodeTPDU(const openpal::RSlice& data);
		void DecodeAPDU(const openpal::RSlice& data);		

	private:
		
		static bool IsResponse(const openpal::RSlice& data);

		/// --- Implement IFrameSink ---
		virtual bool OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata) override;

		/// --- Implement IAPDUHandler ---

		virtual bool IsAllowed(uint32_t, GroupVariation, QualifierCode) override { return true; }

		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var1& value, const openpal::RSlice& object) override;
		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var2& value, const openpal::RSlice& object) override;
		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var5& value, const openpal::RSlice& object) override;
		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var6& value, const openpal::RSlice& object) override;
		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var7& value, const openpal::RSlice& object) override;
		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var10& value, const openpal::RSlice& object) override;
		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var11& value, const openpal::RSlice& object) override;
		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var12& value, const openpal::RSlice& object) override;
		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var13& value, const openpal::RSlice& object) override;
		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var14& value, const openpal::RSlice& object) override;
		virtual IINField ProcessHeader(const FreeFormatHeader& header, const Group120Var15& value, const openpal::RSlice& object) override;


		virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group50Var1>& values) override;
		virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group51Var1>& values) override;
		virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group51Var2>& values) override;
		virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group52Var1>& values) override;
		virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group52Var2>& values) override;
		virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group120Var4>& values) override;

		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<IINValue>>& values) override;
		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Binary>>& values) override;
		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values) override;
		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values) override;
		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Counter>>& values) override;
		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<FrozenCounter>>& values) override;
		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Analog>>& values) override;
		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values) override;
		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<OctetString>>& values) override;
		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<TimeAndInterval>>& values) override;
		virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Group121Var1>>& values) override;

		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<FrozenCounter>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<OctetString>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<TimeAndInterval>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<BinaryCommandEvent>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogCommandEvent>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Group122Var1>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Group122Var2>>& values) override;

		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<ControlRelayOutputBlock>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt16>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt32>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputFloat32>>& values) override;
		virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputDouble64>>& values) override;


		IDecoderCallbacks* callbacks;
		openpal::Logger logger;		
		LinkLayerParser link;
		TransportRx transportRx;
	};


}

#endif
