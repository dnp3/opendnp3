/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef OPENDNP3_LINK_CONTEXT_H
#define OPENDNP3_LINK_CONTEXT_H

#include <openpal/container/Settable.h>
#include <openpal/container/StaticBuffer.h>
#include <openpal/executor/IExecutor.h>
#include <openpal/executor/TimerRef.h>
#include <openpal/logging/Logger.h>

#include "opendnp3/StackStatistics.h"
#include "opendnp3/gen/LinkStatus.h"
#include "opendnp3/link/ILinkLayer.h"
#include "opendnp3/link/ILinkListener.h"
#include "opendnp3/link/ILinkSession.h"
#include "opendnp3/link/ILinkTx.h"
#include "opendnp3/link/LinkLayerConfig.h"
#include "opendnp3/link/LinkLayerConstants.h"

namespace opendnp3
{

class PriStateBase;
class SecStateBase;

enum class LinkTransmitMode : uint8_t
{
    Idle,
    Primary,
    Secondary
};

//	@section desc Implements the contextual state of DNP3 Data Link Layer
class LinkContext
{

public:
    LinkContext(const openpal::Logger& logger,
                const std::shared_ptr<openpal::IExecutor>&,
                std::shared_ptr<IUpperLayer>,
                std::shared_ptr<opendnp3::ILinkListener>,
                ILinkSession& session,
                const LinkLayerConfig&);

    // ---- helpers for dealing with the FCB bits ----

    void ResetReadFCB()
    {
        nextReadFCB = true;
    }
    void ResetWriteFCB()
    {
        nextWriteFCB = true;
    }
    void ToggleReadFCB()
    {
        nextReadFCB = !nextReadFCB;
    }
    void ToggleWriteFCB()
    {
        nextWriteFCB = !nextWriteFCB;
    }

    // --- helpers for dealing with layer state transitations ---
    bool OnLowerLayerUp();
    bool OnLowerLayerDown();
    bool OnTxReady();
    bool SetTxSegment(ITransportSegment& segments);

    // --- helpers for formatting user data messages ---
    openpal::RSlice FormatPrimaryBufferWithUnconfirmed(const Addresses& addr, const openpal::RSlice& tpdu);
    openpal::RSlice FormatPrimaryBufferWithConfirmed(const Addresses& addr, const openpal::RSlice& tpdu, bool FCB);

    // --- Helpers for queueing frames ---
    void QueueAck(uint16_t destination);
    void QueueLinkStatus(uint16_t destination);
    void QueueResetLinks(uint16_t destination);
    void QueueRequestLinkStatus(uint16_t destination);

    void QueueTransmit(const openpal::RSlice& buffer, bool primary);

    // --- public members ----

    void ResetRetry();
    bool Retry();
    void PushDataUp(const Message& message);
    void CompleteSendOperation();
    void TryStartTransmission();
    void OnKeepAliveTimeout();
    void OnResponseTimeout();
    void StartResponseTimer();
    void StartKeepAliveTimer(const openpal::MonotonicTimestamp& expiration);
    void CancelTimer();
    void FailKeepAlive(bool timeout);
    void CompleteKeepAlive();
    bool OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata);
    bool TryPendingTx(openpal::Settable<openpal::RSlice>& pending, bool primary);

    // buffers used for primary and secondary requests
    openpal::StaticBuffer<LPDU_MAX_FRAME_SIZE> priTxBuffer;
    openpal::StaticBuffer<LPDU_HEADER_SIZE> secTxBuffer;

    openpal::Settable<openpal::RSlice> pendingPriTx;
    openpal::Settable<openpal::RSlice> pendingSecTx;

    openpal::Logger logger;
    const LinkLayerConfig config;
    ITransportSegment* pSegments;
    LinkTransmitMode txMode;
    uint32_t numRetryRemaining;

    const std::shared_ptr<openpal::IExecutor> executor;

    openpal::TimerRef rspTimeoutTimer;
    openpal::TimerRef keepAliveTimer;
    bool nextReadFCB;
    bool nextWriteFCB;
    bool isOnline;
    bool isRemoteReset;
    bool keepAliveTimeout;
    openpal::MonotonicTimestamp lastMessageTimestamp;
    StackStatistics::Link statistics;

    ILinkTx* linktx = nullptr;

    PriStateBase* pPriState;
    SecStateBase* pSecState;

    const std::shared_ptr<opendnp3::ILinkListener> listener;
    const std::shared_ptr<IUpperLayer> upper;

    ILinkSession* pSession;
};

} // namespace opendnp3

#endif
