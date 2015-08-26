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
#ifndef OPENDNP3_LINKLAYER_H
#define OPENDNP3_LINKLAYER_H

#include "LinkContext.h"

namespace opendnp3
{

//	@section desc Implements the contextual state of DNP3 Data Link Layer
class LinkLayer final : public ILinkLayer, public ILinkSession, public HasUpperLayer
{

public:

	LinkLayer(openpal::LogRoot&, openpal::IExecutor&, opendnp3::ILinkListener&, const LinkConfig&);

	void SetRouter(ILinkRouter&);

	// ILinkSession interface
	virtual bool OnLowerLayerUp() override;
	virtual bool OnLowerLayerDown() override;
	virtual bool OnTransmitResult(bool success) override;

	// IFrameSink interface
	virtual bool OnFrame(LinkFunction func, bool isMaster, bool fcb, bool fcvdfc, uint16_t dest, uint16_t source, const openpal::RSlice& userdata = openpal::RSlice()) override;

	// ------------- ILinkLayer --------------------
	virtual void Send(ITransportSegment& segments) override;

	// Functions called by the primary and secondary station states	
	void PostStatusCallback(opendnp3::LinkStatus status);	
	void CompleteSendOperation(bool success);
	void TryStartTransmission();
	void FailKeepAlive(bool timeout);
	void CompleteKeepAlive();		
	void PushDataUp(const openpal::RSlice& data);

	// The full state
	LinkContext ctx;

	void StartTimer();
	void CancelTimer();

private:
		
	bool OnFrameImpl(LinkFunction func, bool isMaster, bool fcb, bool fcvdfc, uint16_t dest, uint16_t source, const openpal::RSlice& userdata);
	void CheckPendingTx(openpal::Settable<openpal::RSlice>& pending, bool primary);
	void OnKeepAliveTimeout();
	void OnResponseTimeout();
	bool Validate(bool isMaster, uint16_t src, uint16_t dest);	
};

}

#endif

