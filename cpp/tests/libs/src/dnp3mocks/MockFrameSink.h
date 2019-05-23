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
#ifndef OPENDNP3_MOCKFRAMESINK_H
#define OPENDNP3_MOCKFRAMESINK_H

#include "DataSink.h"

#include <opendnp3/gen/LinkFunction.h>
#include <opendnp3/link/ILinkSession.h>
#include <opendnp3/link/LinkLayerConstants.h>

#include <functional>
#include <queue>

namespace opendnp3
{

class MockFrameSink : public ILinkSession
{
public:
    MockFrameSink();

    // ILinkSession members
    bool OnLowerLayerUp() override;
    bool OnLowerLayerDown() override;
    bool OnTxReady() override;

    virtual bool OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata) override final;

    void Reset();

    bool CheckLast(LinkFunction func, bool aIsMaster, uint16_t aDest, uint16_t aSrc);
    bool CheckLastWithFCB(LinkFunction func, bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc);
    bool CheckLastWithDFC(LinkFunction func, bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);

    // Last frame information
    size_t m_num_frames;
    LinkHeaderFields m_last_header;

    bool mLowerOnline;

    // Add a function to execute the next time a frame is received
    // This allows us to test re-entrant behaviors
    void AddAction(const std::function<void()>& fun);

    DataSink received;

private:
    // Executes one action, if one is available
    void ExecuteAction();

    std::deque<std::function<void()>> m_actions;

    void Update(LinkFunction aCode, bool aIsMaster, uint16_t aSrc, uint16_t aDest);
};

} // namespace opendnp3

#endif
