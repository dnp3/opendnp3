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
#ifndef OPENDNP3_DECODERIMPL_H
#define OPENDNP3_DECODERIMPL_H

#include "link/IFrameSink.h"
#include "link/LinkLayerParser.h"
#include "transport/TransportRx.h"

#include "opendnp3/decoder/IDecoderCallbacks.h"
#include "opendnp3/logging/Logger.h"

#include <ser4cpp/container/SequenceTypes.h>

namespace opendnp3
{

class DecoderImpl;

// stand-alone DNP3 decoder
class DecoderImpl final : private IFrameSink
{
public:
    DecoderImpl(IDecoderCallbacks& callbacks, const Logger& logger);

    void DecodeLPDU(const ser4cpp::rseq_t& data);
    void DecodeTPDU(const ser4cpp::rseq_t& data);
    void DecodeAPDU(const ser4cpp::rseq_t& data);

private:
    static bool IsResponse(const ser4cpp::rseq_t& data);

    /// --- Implement IFrameSink ---
    virtual bool OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata) override;

    IDecoderCallbacks* callbacks;
    Logger logger;
    LinkLayerParser link;
    TransportRx transportRx;
};

} // namespace opendnp3

#endif
