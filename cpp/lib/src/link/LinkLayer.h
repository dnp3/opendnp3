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
#ifndef OPENDNP3_LINKLAYER_H
#define OPENDNP3_LINKLAYER_H

#include "LinkContext.h"
#include "LinkLayerConfig.h"

namespace opendnp3
{

//	@section desc Implements the contextual state of DNP3 Data Link Layer
class LinkLayer final : public ILinkLayer, public ILinkSession
{

public:
    LinkLayer(const Logger& logger,
              const std::shared_ptr<exe4cpp::IExecutor>&,
              const std::shared_ptr<IUpperLayer>& upper,
              const std::shared_ptr<ILinkListener>&,
              const LinkLayerConfig&);

    void SetRouter(ILinkTx&);

    // ---- Events from below: ILinkSession / IFrameSink  ----

    virtual bool OnLowerLayerUp() override;
    virtual bool OnLowerLayerDown() override;
    virtual bool OnTxReady() override;
    virtual bool OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata) override;

    // ---- Events from above: ILinkLayer ----

    virtual bool Send(ITransportSegment& segments) override;

    const StackStatistics::Link& GetStatistics() const;

private:
    // The full state
    LinkContext ctx;
};

} // namespace opendnp3

#endif
