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
#include "APDUWrapper.h"

#include "AppControlField.h"

#include <cassert>

namespace opendnp3
{

APDUWrapper::APDUWrapper() : valid(false) {}

APDUWrapper::APDUWrapper(const ser4cpp::wseq_t& buffer) : valid(true), buffer(buffer), remaining(buffer)
{
    assert(buffer.length() >= 2); // need a control & function at a minimum
    remaining.advance(2);
}

bool APDUWrapper::IsValid() const
{
    return valid;
}

HeaderWriter APDUWrapper::GetWriter()
{
    return HeaderWriter(&remaining);
}

size_t APDUWrapper::Remaining() const
{
    return remaining.length();
}

void APDUWrapper::SetFunction(FunctionCode code)
{
    assert(buffer.is_not_empty());
    buffer[1] = FunctionCodeSpec::to_type(code);
}

FunctionCode APDUWrapper::GetFunction() const
{
    assert(buffer.is_not_empty());
    return FunctionCodeSpec::from_type(buffer[1]);
}

AppControlField APDUWrapper::GetControl() const
{
    assert(buffer.is_not_empty());
    return AppControlField(buffer[0]);
}

void APDUWrapper::SetControl(AppControlField control)
{
    buffer[0] = control.ToByte();
}

size_t APDUWrapper::Size() const
{
    return buffer.length() - remaining.length();
}

ser4cpp::rseq_t APDUWrapper::ToRSeq() const
{
    return buffer.readonly().take(this->Size());
}

} // namespace opendnp3
