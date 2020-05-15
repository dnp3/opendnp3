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
#ifndef OPENDNP3_OUTSTATIONCONTEXT_H
#define OPENDNP3_OUTSTATIONCONTEXT_H

#include "LayerInterfaces.h"
#include "link/LinkLayerConstants.h"
#include "outstation/ControlState.h"
#include "outstation/Database.h"
#include "outstation/DeferredRequest.h"
#include "outstation/OutstationChannelStates.h"
#include "outstation/OutstationSeqNum.h"
#include "outstation/OutstationStates.h"
#include "outstation/ParsedRequest.h"
#include "outstation/RequestHistory.h"
#include "outstation/ResponseContext.h"
#include "outstation/TimeSyncState.h"
#include "outstation/event/EventBuffer.h"

#include "opendnp3/link/Addresses.h"
#include "opendnp3/logging/Logger.h"
#include "opendnp3/outstation/ICommandHandler.h"
#include "opendnp3/outstation/IOutstationApplication.h"
#include "opendnp3/outstation/OutstationConfig.h"

#include <ser4cpp/container/Pair.h>
#include <ser4cpp/container/Settable.h>

#include <exe4cpp/IExecutor.h>

namespace opendnp3
{

///
/// Represent all of the mutable state in an outstation
///
class OContext : public IUpperLayer
{
    friend class StateIdle;
    friend class StateSolicitedConfirmWait;
    friend class StateUnsolicitedConfirmWait;
    friend class StateNullUnsolicitedConfirmWait;

public:
    OContext(const Addresses& addresses,
             const OutstationConfig& config,
             const DatabaseConfig& db_config,
             const Logger& logger,
             const std::shared_ptr<exe4cpp::IExecutor>& executor,
             std::shared_ptr<ILowerLayer> lower,
             std::shared_ptr<ICommandHandler> commandHandler,
             std::shared_ptr<IOutstationApplication> application);

    /// ----- Implement IUpperLayer ------

    virtual bool OnLowerLayerUp() override;

    virtual bool OnLowerLayerDown() override;

    virtual bool OnTxReady() override final;

    virtual bool OnReceive(const Message& message) override final;

    /// --- Other public members ----

    void HandleNewEvents();

    IUpdateHandler& GetUpdateHandler();

    void SetRestartIIN();

private:
    /// ---- Helper functions that operate on the current state, and may return a new state ----

    OutstationState& ContinueMultiFragResponse(const Addresses& addresses, const AppSeqNum& seq);

    OutstationState& RespondToReadRequest(const ParsedRequest& request);

    OutstationState& ProcessNewRequest(const ParsedRequest& request);

    OutstationState& OnReceiveSolRequest(const ParsedRequest& request);

    OutstationState& RespondToNonReadRequest(const ParsedRequest& request);

    // ---- Processing functions --------

    bool ProcessMessage(const Message& message);

    bool ProcessObjects(const ParsedRequest& request);

    bool ProcessRequest(const ParsedRequest& request);

    bool ProcessBroadcastRequest(const ParsedRequest& request);

    bool ProcessRequestNoAck(const ParsedRequest& request);

    bool ProcessConfirm(const ParsedRequest& request);

    // ---- common helper methods ----

    OutstationState& BeginResponseTx(uint16_t destination, APDUResponse& response);

    void BeginRetransmitLastResponse(uint16_t destination);

    void BeginRetransmitLastUnsolicitedResponse();

    void BeginUnsolTx(APDUResponse& response);

    void BeginTx(uint16_t destination, const ser4cpp::rseq_t& message);

    void CheckForTaskStart();

    void CheckForDeferredRequest();

    void CheckForUnsolicitedNull();

    void CheckForUnsolicited();

    bool ProcessDeferredRequest(const ParsedRequest& request);

    void RestartSolConfirmTimer();

    void RestartUnsolConfirmTimer();

    bool CanTransmit() const;

    IINField GetResponseIIN();

    IINField GetDynamicIIN();

    void UpdateLastBroadcastMessageReceived(uint16_t destination);

    void CheckForBroadcastConfirmation(APDUResponse& response);

    /// --- methods for handling app-layer functions ---

    /// Handles non-read function codes that require a response. builds the response using the supplied writer.
    /// @return An IIN field indicating the validity of the request, and to be returned in the response.
    IINField HandleNonReadResponse(const APDUHeader& header, const ser4cpp::rseq_t& objects, HeaderWriter& writer);

    /// Handles read function codes. May trigger an unsolicited response
    /// @return an IIN field and a partial AppControlField (missing sequence info)
    ser4cpp::Pair<IINField, AppControlField> HandleRead(const ser4cpp::rseq_t& objects, HeaderWriter& writer);

    // ------ Function Handlers ------

    IINField HandleWrite(const ser4cpp::rseq_t& objects);
    IINField HandleSelect(const ser4cpp::rseq_t& objects, HeaderWriter& writer);
    IINField HandleOperate(const ser4cpp::rseq_t& objects, HeaderWriter& writer);
    IINField HandleDirectOperate(const ser4cpp::rseq_t& objects, OperateType opType, HeaderWriter* pWriter);
    IINField HandleDelayMeasure(const ser4cpp::rseq_t& objects, HeaderWriter& writer);
    IINField HandleRecordCurrentTime();
    IINField HandleRestart(const ser4cpp::rseq_t& objects, bool isWarmRestart, HeaderWriter* pWriter);
    IINField HandleAssignClass(const ser4cpp::rseq_t& objects);
    IINField HandleDisableUnsolicited(const ser4cpp::rseq_t& objects, HeaderWriter* writer);
    IINField HandleEnableUnsolicited(const ser4cpp::rseq_t& objects, HeaderWriter* writer);
    IINField HandleCommandWithConstant(const ser4cpp::rseq_t& objects, HeaderWriter& writer, CommandStatus status);
    IINField HandleFreeze(const ser4cpp::rseq_t& objects);
    IINField HandleFreezeAndClear(const ser4cpp::rseq_t& objects);

    // ------ resources --------
    const Addresses addresses;
    Logger logger;
    const std::shared_ptr<exe4cpp::IExecutor> executor;
    const std::shared_ptr<ILowerLayer> lower;
    const std::shared_ptr<ICommandHandler> commandHandler;
    const std::shared_ptr<IOutstationApplication> application;

    // ------ Database, event buffer, and response tracking
    EventBuffer eventBuffer;
    Database database;
    ResponseContext rspContext;

    // ------ Static configuration -------
    OutstationParams params;

    // ------ Shared dynamic state --------
    bool isOnline;
    bool isTransmitting;
    IINField staticIIN;
    exe4cpp::Timer confirmTimer;
    RequestHistory history;
    DeferredRequest deferred;

    // ------ Dynamic state related to controls ------
    ControlState control;

    // ------ Dynamic state related to time synchronization ------
    TimeSyncState time;

    // ------ Dynamic state related to solicited and unsolicited modes ------
    OutstationSolState sol;
    OutstationUnsolState unsol;
    NumRetries unsolRetries;
    bool shouldCheckForUnsolicited;
    OutstationState* state = &StateIdle::Inst();

    // ------ Dynamic state related to broadcast messages ------
    ser4cpp::Settable<LinkBroadcastAddress> lastBroadcastMessageReceived;
};

} // namespace opendnp3

#endif
