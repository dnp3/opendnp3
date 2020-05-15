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
#ifndef OPENDNP3_OUTSTATIONSTATES_H
#define OPENDNP3_OUTSTATIONSTATES_H

#include "outstation/ParsedRequest.h"

#include "opendnp3/util/Uncopyable.h"

namespace opendnp3
{

class OContext;

/**
 * Base class for the outstation states
 */
class OutstationState : private Uncopyable
{
public:
    virtual bool IsIdle()
    {
        return false;
    }

    virtual const char* Name() = 0;

    virtual OutstationState& OnConfirm(OContext&, const ParsedRequest& request) = 0;

    virtual OutstationState& OnConfirmTimeout(OContext&) = 0;

    virtual OutstationState& OnNewReadRequest(OContext&, const ParsedRequest& request) = 0;

    virtual OutstationState& OnNewNonReadRequest(OContext&, const ParsedRequest& request) = 0;

    virtual OutstationState& OnRepeatNonReadRequest(OContext&, const ParsedRequest& request) = 0;

    virtual OutstationState& OnRepeatReadRequest(OContext&, const ParsedRequest& request) = 0;

    virtual OutstationState& OnBroadcastMessage(OContext&, const ParsedRequest& request) = 0;
};

class StateIdle final : public OutstationState
{

public:
    bool IsIdle() final
    {
        return true;
    }

    const char* Name() final
    {
        return "Idle";
    }

    inline static OutstationState& Inst()
    {
        return instance;
    }

    OutstationState& OnConfirm(OContext&, const ParsedRequest& request) final;

    OutstationState& OnConfirmTimeout(OContext&) final;

    OutstationState& OnNewReadRequest(OContext&, const ParsedRequest& request) final;

    OutstationState& OnNewNonReadRequest(OContext&, const ParsedRequest& request) final;

    OutstationState& OnRepeatNonReadRequest(OContext&, const ParsedRequest& request) final;

    OutstationState& OnRepeatReadRequest(OContext&, const ParsedRequest& request) final;

    OutstationState& OnBroadcastMessage(OContext&, const ParsedRequest& request) final;

private:
    static StateIdle instance;

    StateIdle() {}
};

/*
 * waiting for a confirm to a solicited read response
 */
class StateSolicitedConfirmWait final : public OutstationState
{

public:
    inline static OutstationState& Inst()
    {
        return instance;
    }

    const char* Name() final
    {
        return "SolicitedConfirmWait";
    }

    OutstationState& OnConfirm(OContext&, const ParsedRequest& request) final;

    OutstationState& OnConfirmTimeout(OContext&) final;

    OutstationState& OnNewReadRequest(OContext&, const ParsedRequest& request) final;

    OutstationState& OnNewNonReadRequest(OContext&, const ParsedRequest& request) final;

    OutstationState& OnRepeatNonReadRequest(OContext&, const ParsedRequest& request) final;

    OutstationState& OnRepeatReadRequest(OContext&, const ParsedRequest& request) final;

    OutstationState& OnBroadcastMessage(OContext&, const ParsedRequest& request) final;

private:
    static StateSolicitedConfirmWait instance;

    StateSolicitedConfirmWait() {}
};

/*
 * waiting for a confirm to an unsolicited read response
 */
class StateUnsolicitedConfirmWait : public OutstationState
{

public:
    inline static OutstationState& Inst()
    {
        return instance;
    }

    virtual const char* Name() override
    {
        return "UnsolicitedConfirmWait";
    }

    virtual OutstationState& OnConfirm(OContext&, const ParsedRequest& request) override;

    virtual OutstationState& OnConfirmTimeout(OContext&) override;

    virtual OutstationState& OnNewReadRequest(OContext&, const ParsedRequest& request) override;

    virtual OutstationState& OnNewNonReadRequest(OContext&, const ParsedRequest& request) override;

    virtual OutstationState& OnRepeatNonReadRequest(OContext&, const ParsedRequest& request) override;

    virtual OutstationState& OnRepeatReadRequest(OContext&, const ParsedRequest& request) override;

    virtual OutstationState& OnBroadcastMessage(OContext&, const ParsedRequest& request) override;

protected:
    static StateUnsolicitedConfirmWait instance;

    StateUnsolicitedConfirmWait() {}
};

/*
 * waiting for a confirm for an unsolicited NULL response (only used in the workaround)
 */
class StateNullUnsolicitedConfirmWait final : public StateUnsolicitedConfirmWait
{

public:
    inline static OutstationState& Inst()
    {
        return instance;
    }

    const char* Name() final
    {
        return "NullUnsolicitedConfirmWait";
    }

    OutstationState& OnNewReadRequest(OContext&, const ParsedRequest& request) final;

private:
    static StateNullUnsolicitedConfirmWait instance;

    StateNullUnsolicitedConfirmWait() {}
};

} // namespace opendnp3

#endif
