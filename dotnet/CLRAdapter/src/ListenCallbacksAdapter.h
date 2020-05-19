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
#ifndef OPENDNP3CLR_LISTEN_CALLBACKS_ADAPTER_H
#define	OPENDNP3CLR_LISTEN_CALLBACKS_ADAPTER_H

#include <opendnp3/master/IListenCallbacks.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            private class ListenCallbacksAdapter final : public opendnp3::IListenCallbacks
            {
            public:

                ListenCallbacksAdapter(Interface::IListenCallbacks^ proxy);

                virtual bool AcceptConnection(uint64_t sessionid, const std::string& ipaddress) override;
                virtual bool AcceptCertificate(uint64_t sessionid, const opendnp3::X509Info& info) override;
                virtual opendnp3::TimeDuration GetFirstFrameTimeout() override;
                virtual void OnFirstFrame(uint64_t sessionid, const opendnp3::LinkHeaderFields& header, opendnp3::ISessionAcceptor& acceptor) override;
                virtual void OnConnectionClose(uint64_t sessionid, const std::shared_ptr<opendnp3::IMasterSession>& session) override;
                virtual void OnCertificateError(uint64_t sessionid, const opendnp3::X509Info& info, int err) override;

            private:

                gcroot < Interface::IListenCallbacks^ > m_proxy;
            };

        }
    }
}

#endif
