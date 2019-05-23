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
#include "ExampleListenCallbacks.h"

#include <opendnp3/master/ISOEHandler.h>

#include <asiodnp3/DefaultMasterApplication.h>

#include <algorithm>
#include <iostream>
#include <sstream>

/**
 * A crude example of a measurement handler on a per-outstation basis
 *
 * All this does is write to the console when a measurement response is received for a particular outstation.
 *
 * A real implementation would map the data and forward, save to a database, etc.
 */
class ExampleSOEHandler : public opendnp3::ISOEHandler
{
public:
    explicit ExampleSOEHandler(const uint16_t address) : address(address) {}

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Binary>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::DoubleBitBinary>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Analog>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Counter>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::FrozenCounter>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryOutputStatus>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputStatus>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::OctetString>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::TimeAndInterval>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryCommandEvent>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogCommandEvent>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::SecurityStat>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::DNPTime>& values) override {}

protected:
    const uint16_t address;

    void Start() override
    {
        std::cout << "Begin receiving measurement data for outstation: " << address << std::endl;
    }

    void End() override
    {
        std::cout << "End receiving measurement data for outstation: " << address << std::endl;
    }
};

std::shared_ptr<asiodnp3::IMasterSession> ExampleListenCallbacks::get_outstation_session(uint16_t address)
{
    std::lock_guard<std::mutex> lock(this->mutex);
    const auto iter = std::find_if(this->sessions.begin(), this->sessions.end(),
                                   [=](const auto& item) { return item.address == address; });
    return (iter == this->sessions.end()) ? nullptr : iter->session;
}

bool ExampleListenCallbacks::AcceptConnection(uint64_t sessionid, const std::string& ipaddress)
{
    // You can optionally establish an IP list filter or verify that IP and DNP3 address match
    return true;
}

bool ExampleListenCallbacks::AcceptCertificate(uint64_t sessionid, const asiodnp3::X509Info& info)
{
    // not relevant since example doesn't use TLS
    return false;
}

openpal::TimeDuration ExampleListenCallbacks::GetFirstFrameTimeout()
{
    // tune this number for your application
    return openpal::TimeDuration::Seconds(5);
}

void ExampleListenCallbacks::OnFirstFrame(uint64_t sessionid,
                                          const opendnp3::LinkHeaderFields& header,
                                          asiodnp3::ISessionAcceptor& acceptor)
{
    std::lock_guard<std::mutex> lock(this->mutex);

    // do we already have a session with this outstation?
    const auto iter = std::find_if(this->sessions.begin(), this->sessions.end(),
                                   [&](const auto& item) { return item.address == header.src; });
    if (iter != this->sessions.end())
    {

        std::cout << "Already connected to outstation w/ address " << header.src << ". Closing first connection."
                  << std::endl;

        // if so, shutdown the existing session
        iter->session->BeginShutdown();
        this->sessions.erase(iter);
    }

    asiodnp3::MasterStackConfig config;

    // use the master and outstation addresses that the outstation is using
    config.link.LocalAddr = header.dest;
    config.link.RemoteAddr = header.src;

    // don't disable unsolicited reporting when the master comes online
    config.master.disableUnsolOnStartup = false;
    // don't perform an integrity scan when the application layer comes online
    config.master.startupIntegrityClassMask = opendnp3::ClassField::None();

    const auto session
        = acceptor.AcceptSession(GetSessionName(header.src, sessionid), std::make_shared<ExampleSOEHandler>(header.src),
                                 std::make_shared<asiodnp3::DefaultMasterApplication>(), config);

    // add to the list
    this->sessions.emplace_back(SessionInfo{sessionid, header.src, session});

    std::cout << "Outstation session start: " << header.src << std::endl;
}

void ExampleListenCallbacks::OnConnectionClose(uint64_t sessionid,
                                               const std::shared_ptr<asiodnp3::IMasterSession>& session)
{
    std::lock_guard<std::mutex> lock(this->mutex);
    const auto iter = std::find_if(this->sessions.begin(), this->sessions.end(),
                                   [=](const auto& item) { return item.id == sessionid; });
    if (iter != this->sessions.end())
    {
        std::cout << "Outstation session close: " << iter->address << std::endl;
        this->sessions.erase(iter);
    }
}

void ExampleListenCallbacks::OnCertificateError(uint64_t sessionid, const asiodnp3::X509Info& info, int error)
{
    // not relevant since example doesn't use TLS
}

std::string ExampleListenCallbacks::GetSessionName(uint16_t outstation_address, uint64_t session_id)
{
    std::ostringstream oss;
    oss << "session[" << session_id << "] to " << outstation_address;
    return oss.str();
}
