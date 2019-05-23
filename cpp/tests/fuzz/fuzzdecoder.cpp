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
#include <openpal/container/Buffer.h>
#include <openpal/logging/Logger.h>

#include <opendnp3/LogLevels.h>

#include <asiodnp3/ConsoleLogger.h>

#include <dnp3decode/Decoder.h>

using namespace std;
using namespace openpal;
using namespace opendnp3;
using namespace asiodnp3;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size)
{
    RSlice buffer(Data, Size);
    openpal::Logger logger(ConsoleLogger::Create(), "decoder", LogFilters(0));
    IDecoderCallbacks callback;
    Decoder decoder(callback, logger);

    decoder.DecodeLPDU(buffer);

    return 0;
}
