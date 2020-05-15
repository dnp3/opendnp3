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
#include "utils/APDUHexBuilders.h"
#include "utils/CallbackQueue.h"
#include "utils/MasterTestFixture.h"
#include "utils/MeasurementComparisons.h"

#include <app/APDUBuilders.h>
#include <app/APDUResponse.h>
#include <catch.hpp>

using namespace opendnp3;

#define SUITE(name) "MasterUnsolTestSuite - " name

TEST_CASE(SUITE("ReceiveUnsolBeforeTransmit"))
{
    MasterParams params;
    params.disableUnsolOnStartup = false;
    MasterTestFixture t(params);

    t.context->OnLowerLayerUp();

    t.SendToMaster(hex::NullUnsolicited(0, IINField::Empty()));

    REQUIRE(t.exe->run_many());

    REQUIRE(t.lower->PopWriteAsHex() == hex::UnsolConfirm(0));
    t.context->OnTxReady();

    REQUIRE(t.lower->PopWriteAsHex() == hex::IntegrityPoll(0));
}
