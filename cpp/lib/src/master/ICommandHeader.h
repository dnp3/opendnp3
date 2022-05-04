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
#ifndef OPENDNP3_ICOMMAND_HEADER_H
#define OPENDNP3_ICOMMAND_HEADER_H

#include "opendnp3/app/AnalogOutput.h"
#include "opendnp3/app/ControlRelayOutputBlock.h"
#include "opendnp3/app/Indexed.h"
#include "opendnp3/app/parsing/ICollection.h"
#include "opendnp3/gen/IndexQualifierMode.h"
#include "opendnp3/master/CommandPointResult.h"
#include "opendnp3/master/HeaderInfo.h"

namespace opendnp3
{

class HeaderWriter;

struct CommandState
{
    CommandState(uint16_t index_) : state(CommandPointState::INIT), status(CommandStatus::UNDEFINED), index(index_) {}

    CommandPointState state;
    CommandStatus status;
    uint16_t index;
};

/**
 * Represents an object header of command objects (CROB or AO)
 */
class ICommandHeader : public ICollection<CommandState>
{
public:
    virtual ~ICommandHeader() {}

    /// Write all of the headers to an ASDU
    virtual bool Write(HeaderWriter&, IndexQualifierMode mode) = 0;

    /// Ask if all of the individual commands have been selected
    virtual bool AreAllSelected() const = 0;

    // --- each overriden classs will only override one of these ---

    virtual void ApplySelectResponse(QualifierCode code, const ICollection<Indexed<ControlRelayOutputBlock>>& commands)
    {
    }
    virtual void ApplySelectResponse(QualifierCode code, const ICollection<Indexed<AnalogOutputInt16>>& commands) {}
    virtual void ApplySelectResponse(QualifierCode code, const ICollection<Indexed<AnalogOutputInt32>>& commands) {}
    virtual void ApplySelectResponse(QualifierCode code, const ICollection<Indexed<AnalogOutputFloat32>>& commands) {}
    virtual void ApplySelectResponse(QualifierCode code, const ICollection<Indexed<AnalogOutputDouble64>>& commands) {}

    // --- each overriden classs will only override one of these ---

    virtual void ApplyOperateResponse(QualifierCode code, const ICollection<Indexed<ControlRelayOutputBlock>>& commands)
    {
    }
    virtual void ApplyOperateResponse(QualifierCode code, const ICollection<Indexed<AnalogOutputInt16>>& commands) {}
    virtual void ApplyOperateResponse(QualifierCode code, const ICollection<Indexed<AnalogOutputInt32>>& commands) {}
    virtual void ApplyOperateResponse(QualifierCode code, const ICollection<Indexed<AnalogOutputFloat32>>& commands) {}
    virtual void ApplyOperateResponse(QualifierCode code, const ICollection<Indexed<AnalogOutputDouble64>>& commands) {}
};

} // namespace opendnp3

#endif
