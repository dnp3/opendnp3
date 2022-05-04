/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements. 
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
#include "MasterAdapter.h"

#include "Conversions.h"

namespace Step Function I/O
{
    namespace DNP3
    {
        namespace Adapter
        {

            MasterAdapter::MasterAdapter(const std::shared_ptr<opendnp3::IMaster>& master)
                : MasterOperationsAdapter(master.get()),
                  master(new std::shared_ptr<opendnp3::IMaster>(master))
            {}

            MasterAdapter::!MasterAdapter()
            {
                delete master;
            }

            void MasterAdapter::Enable()
            {
                (*master)->Enable();
            }

            void MasterAdapter::Disable()
            {
                (*master)->Disable();
            }

            void MasterAdapter::Shutdown()
            {
                (*master)->Shutdown();
            }

            Interface::IStackStatistics^ MasterAdapter::GetStackStatistics()
            {
                return Conversions::ConvertStackStats((*master)->GetStackStatistics());
            }

        }
    }
}
