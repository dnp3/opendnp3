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
#ifndef ASIOPAL_IASYNCCHANNEL_H
#define ASIOPAL_IASYNCCHANNEL_H

#include <functional>
#include <system_error>

#include <openpal/container/WSlice.h>
#include <openpal/container/RSlice.h>
#include <openpal/util/Uncopyable.h>

namespace asiopal
{
typedef std::function<void (const std::error_code& ec, std::size_t num)> ReadCallbackT;
typedef std::function<void (const std::error_code& ec, std::size_t num)> WriteCallbackT;
typedef std::function<void ()> ShutdownCallbackT;

class IAsyncChannel : private openpal::Uncopyable
{
public:
	virtual ~IAsyncChannel() {}

	virtual void BeginRead(openpal::WSlice& buffer, const ReadCallbackT& callback) = 0;
	virtual void BeginWrite(const openpal::RSlice& buffer, const WriteCallbackT& callback) = 0;
	virtual void BeginShutdown(const ShutdownCallbackT& callback) = 0;
};
}

#endif
