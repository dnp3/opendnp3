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
#include "mocks/MockTLSPair.h"

#include <catch.hpp>

#include <fstream>
#include <iostream>

using namespace opendnp3;

#define SUITE(name) "TLS client/server suite - " name

template<class F> void WithIO(const F& fun)
{
    auto io = std::make_shared<MockIO>();
    fun(io);
    io->RunUntilOutOfWork();
}

bool exists(const std::string& file)
{
    std::ifstream infile(file);
    return infile.good();
}

TEST_CASE(SUITE("client and server can connect using self-signed certificate and peer certifcate for verification"))
{
    const auto key1 = "certs/self_signed/entity1_key.pem";
    const auto key2 = "certs/self_signed/entity2_key.pem";
    const auto cert1 = "certs/self_signed/entity1_cert.pem";
    const auto cert2 = "certs/self_signed/entity2_cert.pem";

    const auto all_certs_found = exists(key1) && exists(key2) && exists(cert1) && exists(cert2);
    REQUIRE(all_certs_found);

    auto iteration = [=]() {
        auto test = [=](const std::shared_ptr<MockIO>& io) {
            TLSConfig cfg1(cert2, cert1, key1);
            TLSConfig cfg2(cert1, cert2, key2);

            MockTLSPair pair(io, 20001, cfg1, cfg2);

            pair.Connect(1);
        };

        WithIO(test);
    };

    // run multiple times to ensure the test is cleaning up after itself in terms of system resources
    for (int i = 0; i < 5; ++i)
    {
        iteration();
    }
}

TEST_CASE(SUITE("client and server can connect using certificates signed by common CA"))
{
    const auto key1 = "certs/ca_chain/entity1_key.pem";
    const auto key2 = "certs/ca_chain/entity2_key.pem";
    const auto cert1 = "certs/ca_chain/entity1_cert.pem";
    const auto cert2 = "certs/ca_chain/entity2_cert.pem";
    const auto ca_cert = "certs/ca_chain/ca_cert.pem";

    const auto all_certs_found = exists(key1) && exists(key2) && exists(cert1) && exists(cert2) && exists(ca_cert);
    REQUIRE(all_certs_found);

    auto iteration = [=]() {
        auto test = [=](const std::shared_ptr<MockIO>& io) {
            TLSConfig cfg1(ca_cert, cert1, key1);
            TLSConfig cfg2(ca_cert, cert2, key2);

            MockTLSPair pair(io, 20001, cfg1, cfg2);

            pair.Connect(1);
        };

        WithIO(test);
    };

    // run multiple times to ensure the test is cleaning up after itself in terms of system resources
    for (int i = 0; i < 5; ++i)
    {
        iteration();
    }
}
