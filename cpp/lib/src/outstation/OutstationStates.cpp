/*
 * Copyright 2013-2020 Automatak, LLC
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

#include "outstation/OutstationStates.h"

#include "logging/LogMacros.h"
#include "outstation/OutstationContext.h"

#include "opendnp3/logging/LogLevels.h"

namespace opendnp3
{

// ------------- StateIdle ----------------

StateIdle StateIdle::instance;

OutstationState& StateIdle::OnConfirm(OContext& ctx, const ParsedRequest& request)
{
    FORMAT_LOG_BLOCK(ctx.logger, flags::WARN, "unexpected confirm while IDLE with sequence: %u",
                     request.header.control.SEQ);
    return StateIdle::Inst();
}

OutstationState& StateIdle::OnConfirmTimeout(OContext& ctx)
{
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "unexpected confirm timeout");
    return StateIdle::Inst();
}

OutstationState& StateIdle::OnNewReadRequest(OContext& ctx, const ParsedRequest& request)
{
    return ctx.RespondToReadRequest(request);
}

OutstationState& StateIdle::OnNewNonReadRequest(OContext& ctx, const ParsedRequest& request)
{
    return ctx.RespondToNonReadRequest(request);
}

OutstationState& StateIdle::OnRepeatNonReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.BeginRetransmitLastResponse(request.addresses.source);
    return *this;
}

OutstationState& StateIdle::OnRepeatReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.BeginRetransmitLastResponse(request.addresses.source);
    return *this;
}

OutstationState& StateIdle::OnBroadcastMessage(OContext& ctx, const ParsedRequest& request)
{
    ctx.ProcessBroadcastRequest(request);
    return *this;
}

// ------------- StateSolicitedConfirmWait ----------------

StateSolicitedConfirmWait StateSolicitedConfirmWait::instance;

OutstationState& StateSolicitedConfirmWait::OnConfirm(OContext& ctx, const ParsedRequest& request)
{
    if (request.header.control.UNS)
    {
        FORMAT_LOG_BLOCK(ctx.logger, flags::WARN,
                         "received unsolicited confirm while waiting for solicited confirm (seq: %u)",
                         request.header.control.SEQ);
        return *this;
    }

    if (!ctx.sol.seq.confirmNum.Equals(request.header.control.SEQ))
    {
        FORMAT_LOG_BLOCK(ctx.logger, flags::WARN, "solicited confirm with wrong seq: %u, expected: %u",
                         request.header.control.SEQ, ctx.sol.seq.confirmNum.Get());
        return *this;
    }

    ctx.history.Reset(); // any time we get a confirm we can treat any request as a new request
    ctx.confirmTimer.cancel();
    ctx.eventBuffer.ClearWritten();
    ctx.lastBroadcastMessageReceived.clear();

    // information the application about the confirm
    ctx.application->OnConfirmProcessed(
        false,
        ctx.eventBuffer.NumEvents(EventClass::EC1),
        ctx.eventBuffer.NumEvents(EventClass::EC2),
        ctx.eventBuffer.NumEvents(EventClass::EC3)
    );

    if (ctx.rspContext.HasSelection())
    {
        return ctx.ContinueMultiFragResponse(request.addresses, AppSeqNum(request.header.control.SEQ).Next());
    }

    return StateIdle::Inst();
}

OutstationState& StateSolicitedConfirmWait::OnConfirmTimeout(OContext& ctx)
{
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "solicited confirm timeout");
    return StateIdle::Inst();
}

OutstationState& StateSolicitedConfirmWait::OnNewReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.confirmTimer.cancel();
    return ctx.RespondToReadRequest(request);
}

OutstationState& StateSolicitedConfirmWait::OnNewNonReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.confirmTimer.cancel();
    return ctx.RespondToNonReadRequest(request);
}

OutstationState& StateSolicitedConfirmWait::OnRepeatNonReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.confirmTimer.cancel();
    ctx.BeginRetransmitLastResponse(request.addresses.source);
    return *this;
}

OutstationState& StateSolicitedConfirmWait::OnRepeatReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.RestartSolConfirmTimer();
    ctx.BeginRetransmitLastResponse(request.addresses.source);
    return *this;
}

OutstationState& StateSolicitedConfirmWait::OnBroadcastMessage(OContext& ctx, const ParsedRequest& request)
{
    ctx.ProcessBroadcastRequest(request);
    return StateIdle::Inst();
}

// ------------- StateUnsolicitedConfirmWait ----------------

StateUnsolicitedConfirmWait StateUnsolicitedConfirmWait::instance;

OutstationState& StateUnsolicitedConfirmWait::OnConfirm(OContext& ctx, const ParsedRequest& request)
{
    if (!request.header.control.UNS)
    {
        FORMAT_LOG_BLOCK(ctx.logger, flags::WARN,
                         "received solicited confirm while waiting for unsolicited confirm (seq: %u)",
                         request.header.control.SEQ);
        return *this;
    }

    if (!ctx.unsol.seq.confirmNum.Equals(request.header.control.SEQ))
    {
        FORMAT_LOG_BLOCK(ctx.logger, flags::WARN, "unsolicited confirm with wrong seq: %u, expected: %u",
                         request.header.control.SEQ, ctx.unsol.seq.confirmNum.Get());
        return *this;
    }

    ctx.history.Reset(); // any time we get a confirm we can treat any request as a new request
    ctx.confirmTimer.cancel();
    ctx.lastBroadcastMessageReceived.clear();

    // information the application about the confirm
    ctx.application->OnConfirmProcessed(
        true,
        ctx.eventBuffer.NumEvents(EventClass::EC1),
        ctx.eventBuffer.NumEvents(EventClass::EC2),
        ctx.eventBuffer.NumEvents(EventClass::EC3)
    );

    if (ctx.unsol.completedNull)
    {
        ctx.eventBuffer.ClearWritten();
    }
    else
    {
        ctx.unsol.completedNull = true;
    }

    ctx.shouldCheckForUnsolicited = true;

    return StateIdle::Inst();
}

OutstationState& StateUnsolicitedConfirmWait::OnConfirmTimeout(OContext& ctx)
{
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "unsolicited confirm timeout");

    if (ctx.unsol.completedNull)
    {
        auto shouldRetry = ctx.unsolRetries.Retry();
        if (shouldRetry && !ctx.deferred.IsSet())
        {
            ctx.RestartUnsolConfirmTimer();
            ctx.BeginRetransmitLastUnsolicitedResponse();
            return *this;
        }
        else
        {
            ctx.eventBuffer.Unselect();
        }
    }

    return StateIdle::Inst();
}

OutstationState& StateUnsolicitedConfirmWait::OnNewReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.deferred.Set(request);
    return *this;
}

OutstationState& StateUnsolicitedConfirmWait::OnNewNonReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.deferred.Reset();
    ctx.RespondToNonReadRequest(request);
    return *this;
}

OutstationState& StateUnsolicitedConfirmWait::OnRepeatNonReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.BeginRetransmitLastResponse(request.addresses.source);
    return *this;
}

OutstationState& StateUnsolicitedConfirmWait::OnRepeatReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.deferred.Set(request);
    return *this;
}

OutstationState& StateUnsolicitedConfirmWait::OnBroadcastMessage(OContext& ctx, const ParsedRequest& request)
{
    ctx.ProcessBroadcastRequest(request);
    return StateIdle::Inst();
}

// ------------- StateUnsolicitedConfirmWait ----------------

StateNullUnsolicitedConfirmWait StateNullUnsolicitedConfirmWait::instance;

OutstationState& StateNullUnsolicitedConfirmWait::OnNewReadRequest(OContext& ctx, const ParsedRequest& request)
{
    ctx.confirmTimer.cancel();
    return ctx.RespondToReadRequest(request);
}

} // namespace opendnp3
