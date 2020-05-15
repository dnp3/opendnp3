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
#ifndef OPENDNP3_SECLINKLAYERSTATES_H
#define OPENDNP3_SECLINKLAYERSTATES_H

#include "link/LinkContext.h"
#include "link/Singleton.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

namespace opendnp3
{

class SecStateBase
{
public:
    // Incoming messages to secondary station

    virtual SecStateBase& OnResetLinkStates(LinkContext&, uint16_t source) = 0;
    virtual SecStateBase& OnRequestLinkStatus(LinkContext&, uint16_t source) = 0;

    virtual SecStateBase& OnTestLinkStatus(LinkContext&, uint16_t source, bool fcb) = 0;
    virtual SecStateBase& OnConfirmedUserData(
        LinkContext&, uint16_t source, bool fcb, bool isBroadcast, const Message& message)
        = 0;

    virtual SecStateBase& OnTxReady(LinkContext& ctx);

    // every concrete state implements this for logging purposes

    virtual char const* Name() const = 0;
};

////////////////////////////////////////////////////////
//	Class SLLS_TransmitWait
////////////////////////////////////////////////////////
template<class NextState> class SLLS_TransmitWaitBase : public SecStateBase
{

protected:
    SLLS_TransmitWaitBase() {}

public:
    virtual SecStateBase& OnTxReady(LinkContext& ctx) override final;
    virtual SecStateBase& OnResetLinkStates(LinkContext&, uint16_t source) override final;
    virtual SecStateBase& OnRequestLinkStatus(LinkContext&, uint16_t source) override final;
    virtual SecStateBase& OnTestLinkStatus(LinkContext&, uint16_t source, bool fcb) override final;
    virtual SecStateBase& OnConfirmedUserData(
        LinkContext&, uint16_t source, bool fcb, bool isBroadcast, const Message& message) override final;
};

template<class NextState> SecStateBase& SLLS_TransmitWaitBase<NextState>::OnTxReady(LinkContext& ctx)
{
    return NextState::Instance();
}

template<class NextState>
SecStateBase& SLLS_TransmitWaitBase<NextState>::OnResetLinkStates(LinkContext& ctx, uint16_t source)
{
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Ignoring link frame, remote is flooding");
    return *this;
}

template<class NextState>
SecStateBase& SLLS_TransmitWaitBase<NextState>::OnRequestLinkStatus(LinkContext& ctx, uint16_t source)
{
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Ignoring link frame, remote is flooding");
    return *this;
}

template<class NextState>
SecStateBase& SLLS_TransmitWaitBase<NextState>::OnTestLinkStatus(LinkContext& ctx, uint16_t source, bool fcb)
{
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Ignoring link frame, remote is flooding");
    return *this;
}

template<class NextState>
SecStateBase& SLLS_TransmitWaitBase<NextState>::OnConfirmedUserData(
    LinkContext& ctx, uint16_t source, bool fcb, bool isBroadcast, const Message& message)
{
    SIMPLE_LOG_BLOCK(ctx.logger, flags::WARN, "Ignoring link frame, remote is flooding");
    return *this;
}

////////////////////////////////////////////////////////
//	Class SLLS_UnReset
////////////////////////////////////////////////////////
class SLLS_NotReset final : public SecStateBase
{
public:
    MACRO_STATE_SINGLETON_INSTANCE(SLLS_NotReset);

    virtual SecStateBase& OnConfirmedUserData(
        LinkContext&, uint16_t source, bool fcb, bool isBroadcast, const Message& message) override;
    virtual SecStateBase& OnResetLinkStates(LinkContext&, uint16_t source) override;
    virtual SecStateBase& OnRequestLinkStatus(LinkContext&, uint16_t source) override;
    virtual SecStateBase& OnTestLinkStatus(LinkContext&, uint16_t source, bool fcb) override;
};

////////////////////////////////////////////////////////
//	Class SLLS_Reset
////////////////////////////////////////////////////////
class SLLS_Reset final : public SecStateBase
{
    MACRO_STATE_SINGLETON_INSTANCE(SLLS_Reset);

    virtual SecStateBase& OnConfirmedUserData(
        LinkContext&, uint16_t source, bool fcb, bool isBroadcast, const Message& message) override;
    virtual SecStateBase& OnResetLinkStates(LinkContext&, uint16_t source) override;
    virtual SecStateBase& OnRequestLinkStatus(LinkContext&, uint16_t source) override;
    virtual SecStateBase& OnTestLinkStatus(LinkContext&, uint16_t source, bool fcb) override;
};

class SLLS_TransmitWaitReset : public SLLS_TransmitWaitBase<SLLS_Reset>
{
    MACRO_STATE_SINGLETON_INSTANCE(SLLS_TransmitWaitReset);
};

class SLLS_TransmitWaitNotReset : public SLLS_TransmitWaitBase<SLLS_NotReset>
{
    MACRO_STATE_SINGLETON_INSTANCE(SLLS_TransmitWaitNotReset);
};

} // namespace opendnp3

#endif
