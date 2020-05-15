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
#ifndef OPENDNP3_COMMAND_SET_OPS_H
#define OPENDNP3_COMMAND_SET_OPS_H

#include "app/HeaderWriter.h"
#include "app/parsing/IAPDUHandler.h"

#include "opendnp3/gen/IndexQualifierMode.h"
#include "opendnp3/logging/Logger.h"
#include "opendnp3/master/CommandResultCallbackT.h"
#include "opendnp3/master/CommandSet.h"

namespace opendnp3
{
/**
 *
 * Has private access to CommandSet
 *
 * Used to reduce the public API surface exposed in includes to users
 */
class CommandSetOps final : public IAPDUHandler, private Uncopyable
{
    enum class Mode : uint8_t
    {
        Select,
        Operate
    };

    CommandSetOps(Mode mode, CommandSet& commands_);

    Mode mode;

public:
    enum class OperateResult : uint8_t
    {
        OK,
        FAIL_PARSE
    };

    enum class SelectResult : uint8_t
    {
        OK,
        FAIL_PARSE,
        FAIL_SELECT
    };

    /// Write the headers to an ASDU
    static bool Write(const CommandSet& set, HeaderWriter& writer, IndexQualifierMode mode);

    /// Invoke the callback for a response
    static void InvokeCallback(const CommandSet& set, TaskCompletion result, const CommandResultCallbackT& callback);

    /**
     * parses a response to a select, applying each received header to the command set
     *
     * @return true if every object in every header was correctly selected, false otherwise
     */
    static SelectResult ProcessSelectResponse(CommandSet& set, const ser4cpp::rseq_t& headers, Logger* logger);

    /**
     * parses a response to an operate (or DO), applying each received header to the command set
     *
     * @return true if parsing was successful, the results are left in the set
     */
    static OperateResult ProcessOperateResponse(CommandSet& set, const ser4cpp::rseq_t& headers, Logger* logger);

private:
    virtual bool IsAllowed(uint32_t headerCount, GroupVariation gv, QualifierCode qc) override;

    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<ControlRelayOutputBlock>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputInt16>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputInt32>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputFloat32>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputDouble64>>& values) override;

    template<class T> IINField ProcessAny(const PrefixHeader& header, const ICollection<Indexed<T>>& values);

    CommandSet* commands;
};

} // namespace opendnp3

#endif
