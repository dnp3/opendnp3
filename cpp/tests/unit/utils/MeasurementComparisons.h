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
#ifndef OPENDNP3_UNITTESTS_MEASUREMENT_COMPARISONS_H
#define OPENDNP3_UNITTESTS_MEASUREMENT_COMPARISONS_H

#include <opendnp3/app/Indexed.h>
#include <opendnp3/app/MeasurementTypes.h>

#include <outstation/Event.h>

template<class T> bool Equals(const opendnp3::TypedMeasurement<T>& lhs, const opendnp3::TypedMeasurement<T>& rhs)
{
    return (lhs.flags.value == rhs.flags.value) && (lhs.value == rhs.value) && (lhs.time == rhs.time);
}

template<class T> bool operator==(const opendnp3::TypedMeasurement<T>& lhs, const opendnp3::TypedMeasurement<T>& rhs)
{
    return Equals(lhs, rhs);
}

template<class T> bool operator!=(const opendnp3::TypedMeasurement<T>& lhs, const opendnp3::TypedMeasurement<T>& rhs)
{
    return !Equals(lhs, rhs);
}

template<class T> bool operator==(const opendnp3::Indexed<T>& lhs, const opendnp3::Indexed<T>& rhs)
{
    return (lhs.value == rhs.value) && (lhs.index == rhs.index);
}

template<class T> bool operator==(const opendnp3::Event<T>& lhs, const opendnp3::Event<T>& rhs)
{
    return (lhs.clazz == rhs.clazz) && (lhs.value == rhs.value) && (lhs.index == rhs.index);
}

#endif
