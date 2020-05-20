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
#ifndef OPENDNP3CLR_DATABASE_ADAPTER_H
#define OPENDNP3CLR_DATABASE_ADAPTER_H

#include <opendnp3/outstation/IUpdateHandler.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            template <class T>
            private ref class DatabaseAdapter: public Automatak::DNP3::Interface::IDatabase
            {
            public:

                DatabaseAdapter(T* handler);
                
                virtual void Update(Binary^ update, System::UInt16 index, EventMode mode);
                virtual void Update(DoubleBitBinary^ update, System::UInt16 index, EventMode mode);				
                virtual void Update(Analog^ update, System::UInt16 index, EventMode mode);
                virtual void Update(Counter^ update, System::UInt16 index, EventMode mode);
                virtual void FreezeCounter(System::UInt16 index, System::Boolean clear, EventMode mode);
                virtual void Update(BinaryOutputStatus^ update, System::UInt16 index, EventMode mode);
                virtual void Update(AnalogOutputStatus^ update, System::UInt16 index, EventMode mode);
                virtual void Update(OctetString^ update, System::UInt16 index, EventMode mode);
                virtual void Update(TimeAndInterval^ update, System::UInt16 index);								
                
            protected:
                
                T* handler;
            };

            template<class T> DatabaseAdapter<T>::DatabaseAdapter(T* handler) : handler(handler) {}

            template<class T> void DatabaseAdapter<T>::Update(Binary ^ update, System::UInt16 index, EventMode mode)
            {
                handler->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode)mode);
            }

            template<class T>
            void DatabaseAdapter<T>::Update(DoubleBitBinary ^ update, System::UInt16 index, EventMode mode)
            {
                handler->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode)mode);
            }

            template<class T>
            void DatabaseAdapter<T>::Update(Analog ^ update, System::UInt16 index, EventMode mode)
            {
                handler->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode)mode);
            }

            template<class T>
            void DatabaseAdapter<T>::Update(Counter ^ update, System::UInt16 index, EventMode mode)
            {
                handler->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode)mode);
            }

            template<class T>
            void DatabaseAdapter<T>::FreezeCounter(System::UInt16 index, System::Boolean clear, EventMode mode)
            {
                handler->FreezeCounter(index, clear, (opendnp3::EventMode)mode);
            }

            template<class T>
            void DatabaseAdapter<T>::Update(BinaryOutputStatus ^ update, System::UInt16 index, EventMode mode)
            {
                handler->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode)mode);
            }

            template<class T>
            void DatabaseAdapter<T>::Update(AnalogOutputStatus ^ update, System::UInt16 index, EventMode mode)
            {
                handler->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode)mode);
            }

            template<class T>
            void DatabaseAdapter<T>::Update(OctetString ^ update, System::UInt16 index, EventMode mode)
            {
                handler->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode)mode);
            }

            template<class T>
            void DatabaseAdapter<T>::Update(TimeAndInterval ^ update, System::UInt16 index)
            {
                handler->Update(Conversions::ConvertMeas(update), index);
            }

        }
    }
}

#endif
