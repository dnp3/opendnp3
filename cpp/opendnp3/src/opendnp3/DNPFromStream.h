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
#ifndef __DNP_FROM_STREAM_H_
#define __DNP_FROM_STREAM_H_

#include <openpal/Visibility.h>

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
	static typename T::DataType ReadQVT(const uint8_t* apPos, const T* apObj);
};

template <typename T>
inline typename T::DataType DNPFromStream::ReadQ(const uint8_t* apPos, const T* apObj)
{
	auto flags = apObj->mFlag.Get(apPos);
	return T::DataType(flags);
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadQT(const uint8_t* apPos, const T* apObj)
{
	auto flags = apObj->mFlag.Get(apPos);
	auto time = apObj->mTime.Get(apPos);
	return T::DataType(flags, time);
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadV(const uint8_t* apPos, const T* apObj)
{	
	auto value = apObj->mValue.Get(apPos);
	return T::DataType(value);	
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadQV(const uint8_t* apPos, const T* apObj)
{
	auto flags = apObj->mFlag.Get(apPos);
	auto value = apObj->mValue.Get(apPos);
	return T::DataType(value, flags);
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadQVT(const uint8_t* apPos, const T* apObj)
{	
	auto flags = apObj->mFlag.Get(apPos);
	auto value = apObj->mValue.Get(apPos);
	auto time = apObj->mTime.Get(apPos);
	return T::DataType(value, flags, time);
}

}

#ifdef WIN32
//disable thes type converstion warnings
#pragma warning ( pop )
#endif

#endif

