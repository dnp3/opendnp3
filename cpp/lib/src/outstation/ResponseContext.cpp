/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
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
#include "ResponseContext.h"

namespace opendnp3
{

ResponseContext::ResponseContext(IResponseLoader& staticLoader, IResponseLoader& eventLoader)
    : fragmentCount(0), pStaticLoader(&staticLoader), pEventLoader(&eventLoader)
{
}

bool ResponseContext::HasSelection() const
{
    return pStaticLoader->HasAnySelection() || pEventLoader->HasAnySelection();
}

void ResponseContext::Reset()
{
    fragmentCount = 0;
}

AppControlField ResponseContext::LoadResponse(HeaderWriter& writer)
{
    bool fir = fragmentCount == 0;
    ++fragmentCount;

    auto startingSize = writer.Remaining();
    bool notFull = pEventLoader->Load(writer);
    bool someEventsWritten = writer.Remaining() < startingSize;

    if (notFull)
    {
        auto fin = pStaticLoader->Load(writer);
        auto con = !fin || someEventsWritten;
        return AppControlField(fir, fin, con, false);
    }

    return AppControlField(fir, false, true, false);
}

} // namespace opendnp3
