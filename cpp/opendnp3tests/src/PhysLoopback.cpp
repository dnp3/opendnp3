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
#include "PhysLoopback.h"

#include <openpal/IPhysicalLayerAsync.h>

using namespace openpal;
using namespace std::chrono;

namespace opendnp3
{

PhysLoopback::PhysLoopback(openpal::Logger aLogger, openpal::IPhysicalLayerAsync* apPhys) :
	Loggable(aLogger),
	PhysicalLayerMonitor(aLogger, apPhys, openpal::TimeDuration::Seconds(5), openpal::TimeDuration::Seconds(5)),
	mBytesReadWritten(0),
	mBuffer(1024)
{

}

void PhysLoopback::StartRead()
{
	WriteBuffer buffer(mBuffer, mBuffer.Size());
	mpPhys->AsyncRead(buffer);
}

void PhysLoopback::_OnReceive(const openpal::ReadOnlyBuffer& arBuffer)
{
	mBytesReadWritten += arBuffer.Size();
	mpPhys->AsyncWrite(arBuffer);
}

void PhysLoopback::_OnSendSuccess(void)
{
	this->StartRead();
}

void PhysLoopback::_OnSendFailure(void)
{

}

void PhysLoopback::OnPhysicalLayerOpenSuccessCallback(void)
{
	this->StartRead();
}

}

