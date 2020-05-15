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
#ifndef OPENDNP3_DNP3SERIALIZER_H
#define OPENDNP3_DNP3SERIALIZER_H

#include "Serializer.h"

#include "opendnp3/app/GroupVariationID.h"

namespace opendnp3
{

template<class T> class DNP3Serializer : public Serializer<T>
{
public:
    DNP3Serializer(GroupVariationID id,
                   size_t size,
                   typename Serializer<T>::read_func_t read_func,
                   typename Serializer<T>::write_func_t write_func)
        : Serializer<T>(size, read_func, write_func), id(id)
    {
    }

    GroupVariationID ID() const
    {
        return id;
    }

private:
    GroupVariationID id;
};

} // namespace opendnp3

#endif
