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
#include "PriLinkLayerStates.h"

#include "link/LinkLayer.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

namespace opendnp3
{

////////////////////////////////////////
// PriStateBase
////////////////////////////////////////

PriStateBase& PriStateBase::OnAck(LinkContext& ctx, bool /*rxBuffFull*/)
{
    ++ctx.statistics.numUnexpectedFrame;
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Frame context not understood");
    return *this;
}

PriStateBase& PriStateBase::OnNack(LinkContext& ctx, bool /*rxBuffFull*/)
{
    ++ctx.statistics.numUnexpectedFrame;
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Frame context not understood");
    return *this;
}

PriStateBase& PriStateBase::OnLinkStatus(LinkContext& ctx, bool /*rxBuffFull*/)
{
    ++ctx.statistics.numUnexpectedFrame;
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Frame context not understood");
    return *this;
}

PriStateBase& PriStateBase::OnNotSupported(LinkContext& ctx, bool /*rxBuffFull*/)
{
    ++ctx.statistics.numUnexpectedFrame;
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Frame context not understood");
    return *this;
}

PriStateBase& PriStateBase::OnTxReady(LinkContext& ctx)
{
    FORMAT_LOG_BLOCK(ctx.logger, flags::ERR, "Invalid action for state: %s", this->Name());
    return *this;
}

PriStateBase& PriStateBase::OnTimeout(LinkContext& ctx)
{
    FORMAT_LOG_BLOCK(ctx.logger, flags::ERR, "Invalid action for state: %s", this->Name());
    return *this;
}

PriStateBase& PriStateBase::TrySendUnconfirmed(LinkContext& /*ctx*/, ITransportSegment& /*unused*/)
{
    return *this;
}

PriStateBase& PriStateBase::TrySendRequestLinkStatus(LinkContext& /*unused*/)
{
    return *this;
}

////////////////////////////////////////////////////////
// Class PLLS_SecNotResetIdle
////////////////////////////////////////////////////////

PLLS_Idle PLLS_Idle::instance;

PriStateBase& PLLS_Idle::TrySendUnconfirmed(LinkContext& ctx, ITransportSegment& segments)
{
    auto first = segments.GetSegment();
    auto output = ctx.FormatPrimaryBufferWithUnconfirmed(segments.GetAddresses(), first);
    ctx.QueueTransmit(output, true);
    return PLLS_SendUnconfirmedTransmitWait::Instance();
}

PriStateBase& PLLS_Idle::TrySendRequestLinkStatus(LinkContext& ctx)
{
    ctx.keepAliveTimeout = false;
    ctx.QueueRequestLinkStatus(ctx.config.RemoteAddr);
    ctx.listener->OnKeepAliveInitiated();
    ctx.StartResponseTimer();
    return PLLS_RequestLinkStatusWait::Instance();
}

////////////////////////////////////////////////////////
// Class SendUnconfirmedTransmitWait
////////////////////////////////////////////////////////

PLLS_SendUnconfirmedTransmitWait PLLS_SendUnconfirmedTransmitWait::instance;

PriStateBase& PLLS_SendUnconfirmedTransmitWait::OnTxReady(LinkContext& ctx)
{
    if (ctx.pSegments->Advance())
    {
        auto output
            = ctx.FormatPrimaryBufferWithUnconfirmed(ctx.pSegments->GetAddresses(), ctx.pSegments->GetSegment());
        ctx.QueueTransmit(output, true);
        return *this;
    }
    // we're done

    ctx.CompleteSendOperation();
    return PLLS_Idle::Instance();
}

////////////////////////////////////////////////////////
// Class PLLS_RequestLinkStatusWait
////////////////////////////////////////////////////////

PLLS_RequestLinkStatusWait PLLS_RequestLinkStatusWait::instance;

PriStateBase& PLLS_RequestLinkStatusWait::OnAck(LinkContext& ctx, bool /*receiveBuffFull*/)
{
    ctx.CancelTimer();
    ctx.FailKeepAlive(false);
    return PLLS_Idle::Instance();
}

PriStateBase& PLLS_RequestLinkStatusWait::OnNack(LinkContext& ctx, bool /*receiveBuffFull*/)
{
    ctx.CancelTimer();
    ctx.FailKeepAlive(false);
    return PLLS_Idle::Instance();
}

PriStateBase& PLLS_RequestLinkStatusWait::OnLinkStatus(LinkContext& ctx, bool /*receiveBuffFull*/)
{
    ctx.CancelTimer();
    ctx.CompleteKeepAlive();
    return PLLS_Idle::Instance();
}

PriStateBase& PLLS_RequestLinkStatusWait::OnNotSupported(LinkContext& ctx, bool /*receiveBuffFull*/)
{
    ctx.CancelTimer();
    ctx.FailKeepAlive(false);
    return PLLS_Idle::Instance();
}

PriStateBase& PLLS_RequestLinkStatusWait::OnTxReady(LinkContext& ctx)
{
    // The request link status was successfully sent
    return *this;
}

PriStateBase& PLLS_RequestLinkStatusWait::OnTimeout(LinkContext& ctx)
{
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Link status request - response timeout");
    ctx.FailKeepAlive(true);
    return PLLS_Idle::Instance();
}

} // namespace opendnp3
