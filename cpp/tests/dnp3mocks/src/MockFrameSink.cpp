/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#include "dnp3mocks/MockFrameSink.h"

using namespace opendnp3;
using namespace ser4cpp;

MockFrameSink::MockFrameSink() : m_num_frames(0), mLowerOnline(false) {}

bool MockFrameSink::OnLowerLayerUp()
{
    mLowerOnline = true;
    return true;
}

bool MockFrameSink::OnLowerLayerDown()
{
    mLowerOnline = false;
    return true;
}

void MockFrameSink::Reset()
{
    this->received.Clear();
    m_num_frames = 0;
}

bool MockFrameSink::CheckLast(LinkFunction func, bool isMaster, uint16_t dest, uint16_t src)
{
    return (m_last_header.func == func) && (isMaster == m_last_header.isFromMaster)
        && (m_last_header.addresses.source == src) && (m_last_header.addresses.destination == dest);
}

bool MockFrameSink::CheckLastWithFCB(LinkFunction func, bool isMaster, bool aFcb, uint16_t dest, uint16_t src)
{
    return (m_last_header.fcb == aFcb) && CheckLast(func, isMaster, dest, src);
}

bool MockFrameSink::CheckLastWithDFC(LinkFunction func, bool isMaster, bool aIsRcvBuffFull, uint16_t dest, uint16_t src)
{
    return (m_last_header.fcvdfc == aIsRcvBuffFull) && CheckLast(func, isMaster, dest, src);
}

bool MockFrameSink::OnTxReady()
{
    return true;
}

bool MockFrameSink::OnFrame(const LinkHeaderFields& header, const rseq_t& userdata)
{
    ++m_num_frames;

    this->m_last_header = header;

    if (userdata.is_not_empty())
    {
        this->received.Write(userdata);
    }

    return true;
}

void MockFrameSink::AddAction(const std::function<void()>& fun)
{
    m_actions.push_back(fun);
}

void MockFrameSink::ExecuteAction()
{
    if (!m_actions.empty())
    {
        auto f = m_actions.front();
        m_actions.pop_front();
        f();
    }
}
