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
#include "ListenCallbacksAdapter.h"

#include "Conversions.h"
#include "SessionAcceptorAdapter.h"
#include "MasterSessionAdapter.h"

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            ListenCallbacksAdapter::ListenCallbacksAdapter(Interface::IListenCallbacks^ proxy) : 
                m_proxy(proxy)
            {}

            bool ListenCallbacksAdapter::AcceptConnection(uint64_t sessionid, const std::string& ipaddress)
            {
                return m_proxy->AcceptConnection(sessionid, Conversions::ConvertString(ipaddress));
            }

            bool ListenCallbacksAdapter::AcceptCertificate(uint64_t sessionid, const opendnp3::X509Info& info)
            {
                return m_proxy->AcceptCertificate(sessionid, Conversions::Convert(info));
            }

            opendnp3::TimeDuration ListenCallbacksAdapter::GetFirstFrameTimeout()
            {
                return Conversions::ConvertTimespan(m_proxy->GetFirstFrameTimeout());
            }

            void ListenCallbacksAdapter::OnFirstFrame(uint64_t sessionid, const opendnp3::LinkHeaderFields& header, opendnp3::ISessionAcceptor& acceptor)
            {
                auto linkheader = Conversions::Convert(header);
                auto adapter = gcnew SessionAcceptorAdapter(acceptor);
                m_proxy->OnFirstFrame(sessionid, linkheader, adapter);
            }

            void ListenCallbacksAdapter::OnConnectionClose(uint64_t sessionid, const std::shared_ptr<opendnp3::IMasterSession>& session)
            {	
                m_proxy->OnConnectionClose(sessionid, session ? gcnew MasterSessionAdapter(session) : nullptr);
            }

            void ListenCallbacksAdapter::OnCertificateError(uint64_t sessionid, const opendnp3::X509Info& info, int error)
            {
                m_proxy->OnCertificateError(sessionid, Conversions::Convert(info), error);
            }

        }
    }
}
