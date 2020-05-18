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
#ifndef OPENDNP3_TYPEDEVENTRECORD_H
#define OPENDNP3_TYPEDEVENTRECORD_H

#include "EventRecord.h"

namespace opendnp3
{

/**
 * Event details that vary by type
 */
template<class T> struct TypedEventRecord
{
    TypedEventRecord() = default;

    TypedEventRecord(const typename T::meas_t& value,
                     typename T::event_variation_t defaultVariation,
                     Node<EventRecord>* record)
        : value(value), defaultVariation(defaultVariation), selectedVariation(defaultVariation), record(record)
    {
    }

    typename T::meas_t value;
    typename T::event_variation_t defaultVariation;
    typename T::event_variation_t selectedVariation;
    Node<EventRecord>* record = nullptr;
};
} // namespace opendnp3

#endif
