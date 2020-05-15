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
#ifndef OPENDNP3_UNITTESTS_MOCKLOWERLAYER_H
#define OPENDNP3_UNITTESTS_MOCKLOWERLAYER_H

#include <LayerInterfaces.h>

#include <queue>
#include <string>

class MockLowerLayer : public opendnp3::ILowerLayer, public opendnp3::HasUpperLayer
{
public:
    void SendUp(const ser4cpp::rseq_t& data, const opendnp3::Addresses& addresses = opendnp3::Addresses());
    void SendUp(const std::string& arHexData, const opendnp3::Addresses& addresses = opendnp3::Addresses());

    void SendComplete();
    void ThisLayerUp();
    void ThisLayerDown();

    bool HasNoData() const;

    size_t NumWrites() const;
    std::string PopWriteAsHex();

    virtual bool BeginTransmit(const opendnp3::Message& message) override final;

private:
    std::queue<opendnp3::Message> sendQueue;
};

#endif
