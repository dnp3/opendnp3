/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef OPENDNP3_MOCKLOWERLAYER_H
#define OPENDNP3_MOCKLOWERLAYER_H

#include <opendnp3/LayerInterfaces.h>

#include <queue>
#include <string>

namespace opendnp3
{

class MockLowerLayer : public ILowerLayer, public HasUpperLayer
{
public:
    void SendUp(const openpal::RSlice& data, const Addresses& addresses = Addresses());
    void SendUp(const std::string& hex, const Addresses& addresses = Addresses());

    void SendComplete();
    void ThisLayerUp();
    void ThisLayerDown();

    bool HasNoData() const;

    size_t NumWrites() const;
    std::string PopWriteAsHex();

    virtual bool BeginTransmit(const Message& buffer) override final;

private:
    std::queue<Message> sendQueue;
};

} // namespace opendnp3

#endif
