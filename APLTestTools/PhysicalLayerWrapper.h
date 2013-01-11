//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __PHYSICAL_LAYER_WRAPPER_H_
#define __PHYSICAL_LAYER_WRAPPER_H_

#include <APL/IPhysicalLayerAsync.h>
#include <APL/Loggable.h>
#include <APL/IHandlerAsync.h>

#include <APL/RandomDouble.h>

namespace apl
{


class PhysicalLayerWrapper : public IPhysicalLayerAsync, public IHandlerAsync
{
public:
	PhysicalLayerWrapper(Logger* apLogger, IPhysicalLayerAsync* apProxy);

	//implement IPhysicalLayerAsync

	bool CanOpen() const { return mpProxy->CanOpen(); }
	bool CanClose() const { return mpProxy->CanClose(); }
	bool CanRead() const { return mpProxy->CanRead(); }
	bool CanWrite() const { return mpProxy->CanWrite(); }

	bool IsReading() const { return mpProxy->IsReading(); }
	bool IsWriting() const { return mpProxy->IsWriting(); }
	bool IsClosing() const { return mpProxy->IsClosing(); }
	bool IsClosed() const { return mpProxy->IsClosed(); }
	bool IsOpening() const { return mpProxy->IsOpening(); }
	bool IsOpen() const { return mpProxy->IsOpen(); }

	std::string ConvertStateToString() const { return mpProxy->ConvertStateToString(); }

	void AsyncOpen();
	void AsyncClose();
	void AsyncWrite(const uint8_t* apData, size_t apSize);
	void AsyncRead(uint8_t* apData, size_t apSize);

	void SetHandler(IHandlerAsync* apHandler);

	// testing helpers
	void SetCorruptionProbability(double aProbability);

private:
	void _OnLowerLayerUp();
	void _OnLowerLayerDown();
	void _OnReceive(const uint8_t*, size_t aSize);
	void _OnSendSuccess();
	void _OnSendFailure();
	void _OnOpenFailure();

	double mCorruptionProbability;
	RandomDouble mRandom;

private:

	IPhysicalLayerAsync* mpProxy;
	IHandlerAsync* mpHandler;
};

}

#endif
