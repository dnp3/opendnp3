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
#ifndef OPENDNP3_WRITECONVERSIONTEMPLATES_H
#define OPENDNP3_WRITECONVERSIONTEMPLATES_H

#include <openpal/util/Uncopyable.h>

#include "opendnp3/app/DownSampling.h"

#include <cstdint>

namespace opendnp3
{
template <class Target, class Source>
struct ConvertQ : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		t.flags = src.quality;
		return t;
	}
};

template <class Target, class Source>
struct ConvertQV : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		t.value = src.value;
		t.flags = src.quality;
		return t;
	}
};

template <class Target, class Source>
struct ConvertV : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		t.value = src.value;
		return t;
	}
};

template <class Target, class Source>
struct ConvertVandTruncate : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		t.value = static_cast<typename Target::ValueType>(src.value);
		return t;
	}
};

template <class Target, class Source>
struct ConvertVRangeCheck : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		DownSampling<typename Source::Type, typename Target::ValueType>::Apply(src.value, t.value);
		return t;
	}
};

template <class Target, class Source, uint8_t Overrange>
struct ConvertQVRangeCheck : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		auto overrange = DownSampling<typename Source::Type, typename Target::ValueType>::Apply(src.value, t.value);
		t.flags = overrange ? Overrange : 0;
		t.flags |= src.quality;
		return t;
	}
};

template <class Target, class Source>
struct ConvertQVandTruncate : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		t.flags = src.quality;
		t.value = static_cast<typename Target::ValueType>(src.value);
		return t;
	}
};

template <class Target, class Source>
struct ConvertQVT : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		t.flags = src.quality;
		t.value = src.value;
		t.time = src.time;
		return t;
	}
};

template <class Target, class Source>
struct ConvertQVTandTruncate : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		t.flags = src.quality;
		t.value = static_cast<typename Target::ValueType>(src.value);
		t.time = src.time;
		return t;
	}
};

template <class Target, class Source, uint8_t Overrange>
struct ConvertQVTRangeCheck : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		auto overrange = DownSampling<typename Source::Type, typename Target::ValueType>::Apply(src.value, t.value);
		t.flags = overrange ? Overrange : 0;
		t.flags |= src.quality;
		t.time = src.time;
		return t;
	}
};

template <class Target, class Source>
struct ConvertQT : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		t.flags = src.quality;
		t.time = src.time;
		return t;
	}
};

template <class Target, class Source, class Downcast>
struct ConvertQTDowncast : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		t.flags = src.quality;
		t.time = static_cast<Downcast>(src.time);
		return t;
	}
};

template <class Target, class Source>
struct ConvertQS : private openpal::PureStatic
{
	static Target Apply(const Source& src)
	{
		Target t;
		t.value = src.value;
		t.status = src.status;
		return t;
	}
};
}

#endif
