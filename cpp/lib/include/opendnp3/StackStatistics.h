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
#ifndef OPENDNP3_STACKSTATISTICS_H
#define OPENDNP3_STACKSTATISTICS_H

#include <cstdint>

namespace opendnp3
{

/**
 * Statistics related to both a master or outstation session
 */
struct StackStatistics
{
    struct Link
    {
        /// number of unexpected frames
        uint64_t numUnexpectedFrame = 0;

        /// frames received w/ wrong master bit
        uint64_t numBadMasterBit = 0;

        /// frames received for an unknown destination
        uint64_t numUnknownDestination = 0;

        /// frames received for an unknown source
        uint64_t numUnknownSource = 0;
    };

    struct Transport
    {
        struct Rx
        {
            /// Number of valid TPDU's received
            uint64_t numTransportRx = 0;

            /// Number of TPDUs dropped due to malformed contents
            uint64_t numTransportErrorRx = 0;

            /// Number of times received data was too big for reassembly buffer
            uint64_t numTransportBufferOverflow = 0;

            /// number of times transport buffer is discard due to new FIR
            uint64_t numTransportDiscard = 0;

            /// number of segments ignored due to bad FIR/FIN or SEQ
            uint64_t numTransportIgnore = 0;
        };

        struct Tx
        {
            /// Number of TPDUs transmitted
            uint64_t numTransportTx = 0;
        };

        Transport() = default;
        Transport(const Rx& rx, const Tx& tx) : rx(rx), tx(tx) {}

        Rx rx;
        Tx tx;
    };

    StackStatistics() = default;

    StackStatistics(const Link& link, const Transport& transport) : link(link), transport(transport) {}

    Link link;
    Transport transport;
};

} // namespace opendnp3

#endif
