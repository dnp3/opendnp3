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
#ifndef __PHYS_LOOPBACK_H_
#define __PHYS_LOOPBACK_H_

#include <asiodnp3/PhysicalLayerMonitor.h>

#include <testlib/CopyableBuffer.h>

namespace opendnp3
{

/**
*	Buffers and sends all bytes received to back on the same layer.
*/
class PhysLoopback : public asiodnp3::PhysicalLayerMonitor
{
public:
	PhysLoopback(openpal::Logger logger, openpal::IExecutor& executor, openpal::IPhysicalLayer*);

	virtual void OnReceive(const openpal::RSlice&) override final;
	virtual void OnSendResult(bool isSuccess) override final;

private:

	size_t mBytesReadWritten;

	testlib::CopyableBuffer mBuffer;


	virtual void OnPhysicalLayerOpenSuccessCallback() override final;

	virtual void OnPhysicalLayerOpenFailureCallback() override final {}
	virtual void OnPhysicalLayerCloseCallback() override final {}

	void StartRead();
};

}

#endif
