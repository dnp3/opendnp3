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
#ifndef OPENDNP3CLR_EVENT_CONVERTER_H
#define OPENDNP3CLR_EVENT_CONVERTER_H

#include <functional>
#include <vector>

#include <vcclr.h>

using namespace System::Collections::ObjectModel;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            template <class T, class U>
            private class EventConverter
            {
            public:

                EventConverter(const std::function<U(T)>& convert_, System::Action<U>^ listener) :
                    convert(convert_),
                    rootListener(listener)
                {

                }

                std::function<void(T)> GetTrigger()
                {
                    auto trigger = [this](T value) { this->OnEvent(value); };
                    return trigger;
                }

                /// How to subscribe on the managed side
                void AddListener(System::Action<U>^ listener)
                {
                    auto pRoot = new gcroot < System::Action<U> ^ >(listener);
                    callbacks.push_back(pRoot);
                }

            private:

                void OnEvent(T value)
                {
                    rootListener->Invoke(convert(value));
                }

                std::function<U(T)> convert;
                gcroot < System::Action<U> ^ > rootListener;
            };

        }
    }
}

#endif
