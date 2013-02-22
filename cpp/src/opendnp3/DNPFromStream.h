
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
#ifndef __DNP_FROM_STREAM_H_
#define __DNP_FROM_STREAM_H_

#include <opendnp3/Visibility.h>

#include <iostream>

#ifdef WIN32
//disable the type converstion warnings
#pragma warning ( push )
#pragma warning ( disable : 4244 )
#endif

namespace opendnp3
{

/** Templates for read dnp3 data types from stream.
	Used in the dnp3 object definitions to define how
	they deserialize themeselves.
*/
class DLL_LOCAL DNPFromStream
{
public:

	//templated conversion functions
	template <typename T>
	static typename T::DataType ReadQ(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadQT(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadV(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadQV(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadBinaryQV(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadBinaryQVT(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadQVT(const uint8_t* apPos, const T* apObj);
};

template <typename T>
inline typename T::DataType DNPFromStream::ReadQ(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQuality(apObj->mFlag.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadBinaryQV(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQualityValue(apObj->mFlag.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadBinaryQVT(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQualityValue(apObj->mFlag.Get(apPos));
	ret.SetTime(apObj->mTime.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadQT(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQuality(apObj->mFlag.Get(apPos));
	ret.SetTime(apObj->mTime.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadV(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetValue(apObj->mValue.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadQV(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQuality(apObj->mFlag.Get(apPos));
	ret.SetValue(apObj->mValue.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadQVT(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQuality(apObj->mFlag.Get(apPos));
	ret.SetValue(apObj->mValue.Get(apPos));
	ret.SetTime(apObj->mTime.Get(apPos));
	return ret;
}

}

#ifdef WIN32
//disable thes type converstion warnings
#pragma warning ( pop )
#endif

#endif

