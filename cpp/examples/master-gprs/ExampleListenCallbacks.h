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
#ifndef EXAMPLE_LISTENCALLBACKS_H
#define EXAMPLE_LISTENCALLBACKS_H

#include <opendnp3/master/IListenCallbacks.h>

#include <mutex>
#include <vector>

class ExampleListenCallbacks final : public opendnp3::IListenCallbacks
{
    struct SessionInfo
    {
        uint64_t id;
        uint16_t address;
        std::shared_ptr<opendnp3::IMasterSession> session;
    };

public:
    // ------ Retrieve a (possibly nullptr) master session reference to control a particular outstation --------

    std::shared_ptr<opendnp3::IMasterSession> get_outstation_session(uint16_t address);

    // ------ Implement IListenCallbacks --------

    bool AcceptConnection(uint64_t sessionid, const std::string& ipaddress) override;

    bool AcceptCertificate(uint64_t sessionid, const opendnp3::X509Info& info) override;

    opendnp3::TimeDuration GetFirstFrameTimeout() override;

    void OnFirstFrame(uint64_t sessionid,
                      const opendnp3::LinkHeaderFields& header,
                      opendnp3::ISessionAcceptor& acceptor) override;

    void OnConnectionClose(uint64_t sessionid, const std::shared_ptr<opendnp3::IMasterSession>& session) override;

    void OnCertificateError(uint64_t sessionid, const opendnp3::X509Info& info, int error) override;

private:
    static std::string GetSessionName(uint16_t outstation_address, uint64_t session_id);

    // synchronize access to all class methods since callbacks can come from library or userland
    std::mutex mutex;

    // more sophisticated implementations might use a map for faster lookup
    std::vector<SessionInfo> sessions;
};

#endif
