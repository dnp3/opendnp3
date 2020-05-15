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
#include "ReadHandler.h"

namespace opendnp3
{

ReadHandler::ReadHandler(IStaticSelector& staticSelector, IEventSelector& eventSelector)
    : pStaticSelector(&staticSelector), pEventSelector(&eventSelector)
{
}

IINField ReadHandler::ProcessHeader(const AllObjectsHeader& header)
{
    switch (header.type)
    {
    case (GroupVariationType::STATIC):
        return pStaticSelector->SelectAll(header.enumeration);
    case (GroupVariationType::EVENT):
        return pEventSelector->SelectAll(header.enumeration);
    default:
        return IINField(IINBit::FUNC_NOT_SUPPORTED);
    }
}

IINField ReadHandler::ProcessHeader(const RangeHeader& header)
{
    return pStaticSelector->SelectRange(header.enumeration, header.range);
}

IINField ReadHandler::ProcessHeader(const CountHeader& header)
{
    return pEventSelector->SelectCount(header.enumeration, header.count);
}

IINField ReadHandler::ProcessHeader(const PrefixHeader& header, const ICollection<uint16_t>& indices)
{
    return pStaticSelector->SelectIndices(header.enumeration, indices);
}

} // namespace opendnp3
