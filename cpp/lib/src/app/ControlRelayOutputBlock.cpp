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
#include "opendnp3/app/ControlRelayOutputBlock.h"

namespace opendnp3
{

ControlRelayOutputBlock::ControlRelayOutputBlock(OperationType opType_,
                                                 TripCloseCode tcc_,
                                                 bool clear_,
                                                 uint8_t count_,
                                                 uint32_t onTime_,
                                                 uint32_t offTime_,
                                                 CommandStatus status_)
    : opType(opType_),
      tcc(tcc_),
      clear(clear_),
      count(count_),
      onTimeMS(onTime_),
      offTimeMS(offTime_),
      status(status_),
      rawCode(((TripCloseCodeSpec::to_type(tcc) << 6) & 0xC0) | ((static_cast<uint8_t>(clear) << 5) & 0x20)
              | (OperationTypeSpec::to_type(opType) & 0x0F))
{
}

ControlRelayOutputBlock::ControlRelayOutputBlock(
    uint8_t rawCode_, uint8_t count_, uint32_t onTime_, uint32_t offTime_, CommandStatus status_)
    : opType(OperationTypeSpec::from_type(rawCode_ & 0x0F)),
      tcc(TripCloseCodeSpec::from_type((rawCode_ >> 6) & 0x3)),
      clear(rawCode_ & 0x20),
      count(count_),
      onTimeMS(onTime_),
      offTimeMS(offTime_),
      status(status_),
      rawCode(rawCode_)
{
}

} // namespace opendnp3
