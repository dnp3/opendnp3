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
#include "SessionAcceptorAdapter.h"

#include "Conversions.h"
#include "MasterApplicationAdapter.h"
#include "MasterSessionAdapter.h"
#include "SOEHandlerAdapter.h"

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            SessionAcceptorAdapter::SessionAcceptorAdapter(opendnp3::ISessionAcceptor& proxy) :
                proxy(&proxy)
            {}

            IMasterSession^ SessionAcceptorAdapter::AcceptSession(
                System::String^ loggerid,
                ISOEHandler^ SOEHandler,
                IMasterApplication^ application,
                MasterStackConfig^ config
                )
            {
                auto id = Conversions::ConvertString(loggerid);
                auto mconfig = Conversions::ConvertConfig(config);	
                auto handler = std::shared_ptr<SOEHandlerAdapter>(new SOEHandlerAdapter(SOEHandler));
                auto app = std::shared_ptr<MasterApplicationAdapter>(new MasterApplicationAdapter(application));
    
                auto session = proxy->AcceptSession(id, handler, app, mconfig);

                if (!session)
                {
                    return nullptr;
                }
    
                return gcnew MasterSessionAdapter(session);
            }

        }
    }
}
