/*
 * Copyright 2013-2019 Automatak, LLC
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

#include <ser4cpp/container/Buffer.h>
#include <log4cpp/Logger.h>

#include <opendnp3/ConsoleLogger.h>
#include <opendnp3/LogLevels.h>
#include <opendnp3/decoder/Decoder.h>

using namespace std;
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
    log4cpp::Logger logger(ConsoleLogger::Create(), log4cpp::ModuleId(), "decoder", log4cpp::LogLevels::everything());
    IDecoderCallbacks callback;
    Decoder decoder(callback, logger);

    ser4cpp::Buffer buffer(4096);

    const Mode MODE = (argc > 1) ? GetMode(argv[1]) : Mode::Link;

    while (true)
    {
        const size_t numRead = fread(buffer.as_wslice(), 1, buffer.length(), stdin);

        if (numRead == 0)
        {
            return 0;
        }

        switch (MODE)
        {
        case (Mode::Link):
            decoder.DecodeLPDU(buffer.as_rslice().take(static_cast<uint32_t>(numRead)));
            break;
        case (Mode::Transport):
            decoder.DecodeTPDU(buffer.as_rslice().take(static_cast<uint32_t>(numRead)));
            break;
        default:
            decoder.DecodeAPDU(buffer.as_rslice().take(static_cast<uint32_t>(numRead)));
            break;
        }
    }

    return 0;
}
