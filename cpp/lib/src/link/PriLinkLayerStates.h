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
#ifndef OPENDNP3_PRILINKLAYERSTATES_H
#define OPENDNP3_PRILINKLAYERSTATES_H

#include "link/Singleton.h"

namespace opendnp3
{

class LinkContext;
class ITransportSegment;

class PriStateBase
{
public:
    // Incoming messages for primary station
    virtual PriStateBase& OnAck(LinkContext&, bool receiveBuffFull);
    virtual PriStateBase& OnNack(LinkContext&, bool receiveBuffFull);
    virtual PriStateBase& OnLinkStatus(LinkContext&, bool receiveBuffFull);
    virtual PriStateBase& OnNotSupported(LinkContext&, bool receiveBuffFull);

    virtual PriStateBase& OnTxReady(LinkContext&);

    virtual PriStateBase& OnTimeout(LinkContext&);

    // transmission events to handle
    virtual PriStateBase& TrySendUnconfirmed(LinkContext&, ITransportSegment& segments);
    virtual PriStateBase& TrySendRequestLinkStatus(LinkContext&);

    // every concrete state implements this for logging purposes
    virtual char const* Name() const = 0;
};

//	@section desc Entry state for primary station
class PLLS_Idle final : public PriStateBase
{
    MACRO_STATE_SINGLETON_INSTANCE(PLLS_Idle);

    PriStateBase& TrySendUnconfirmed(LinkContext&, ITransportSegment& segments) override;
    PriStateBase& TrySendRequestLinkStatus(LinkContext&) override;
};

/////////////////////////////////////////////////////////////////////////////
// Wait state for send unconfirmed data
/////////////////////////////////////////////////////////////////////////////

class PLLS_SendUnconfirmedTransmitWait final : public PriStateBase
{
    MACRO_STATE_SINGLETON_INSTANCE(PLLS_SendUnconfirmedTransmitWait);

    virtual PriStateBase& OnTxReady(LinkContext& ctx) override;
};

/////////////////////////////////////////////////////////////////////////////
// Waiting for a link status response
/////////////////////////////////////////////////////////////////////////////

//	@section desc As soon as we get an ACK, send the delayed pri frame
class PLLS_RequestLinkStatusWait final : public PriStateBase
{
    MACRO_STATE_SINGLETON_INSTANCE(PLLS_RequestLinkStatusWait);

    PriStateBase& OnAck(LinkContext& ctx, bool) override;
    PriStateBase& OnNack(LinkContext& ctx, bool) override;
    PriStateBase& OnLinkStatus(LinkContext& ctx, bool) override;
    PriStateBase& OnNotSupported(LinkContext& ctx, bool) override;
    PriStateBase& OnTxReady(LinkContext&) override;
    PriStateBase& OnTimeout(LinkContext&) override;
};

} // namespace opendnp3

#endif
