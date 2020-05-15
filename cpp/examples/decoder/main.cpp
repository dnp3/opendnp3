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

#include <opendnp3/ConsoleLogger.h>
#include <opendnp3/decoder/Decoder.h>
#include <opendnp3/logging/LogLevels.h>

#include <array>

using namespace opendnp3;

enum class Mode
{
    Link,
    Transport,
    App
};

Mode GetMode(const std::string& mode)
{
    if (mode == "link")
    {
        return Mode::Link;
    }
    else if (mode == "transport")
    {
        return Mode::Transport;
    }
    else
    {
        return Mode::App;
    }
}

int main(int argc, char* argv[])
{
    Logger logger(ConsoleLogger::Create(), ModuleId(), "decoder", LogLevels::everything());
    IDecoderCallbacks callback;
    Decoder decoder(callback, logger);

    std::array<uint8_t, 4096> rawBuffer;

    const Mode MODE = (argc > 1) ? GetMode(argv[1]) : Mode::Link;

    while (true)
    {
        const size_t numRead = fread(rawBuffer.data(), 1, rawBuffer.size(), stdin);

        if (numRead == 0)
        {
            return 0;
        }

        switch (MODE)
        {
        case (Mode::Link):
            decoder.DecodeLPDU(Buffer(rawBuffer.data(), numRead));
            break;
        case (Mode::Transport):
            decoder.DecodeTPDU(Buffer(rawBuffer.data(), numRead));
            break;
        default:
            decoder.DecodeAPDU(Buffer(rawBuffer.data(), numRead));
            break;
        }
    }

    return 0;
}
