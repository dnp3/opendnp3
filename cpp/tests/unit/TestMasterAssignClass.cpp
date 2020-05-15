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
#include "utils/MasterTestFixture.h"

#include <ser4cpp/util/HexConversions.h>

#include <catch.hpp>

using namespace opendnp3;

#define SUITE(name) "MasterAssignClassTestSuite - " name

TEST_CASE(SUITE("AssignsClassAfterConnect"))
{
    MasterTestFixture t(NoStartupTasks());

    // configure the mock application to do assign class on startup
    t.application->assignClassHeaders.push_back(Header::AllObjects(60, 2));
    t.application->assignClassHeaders.push_back(Header::AllObjects(3, 0));

    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex() == "C0 16 3C 02 06 03 00 06");
    t.context->OnTxReady();
    t.SendToMaster("C0 81 00 00");
    t.exe->run_many();

    REQUIRE(t.context->tstate == MContext::TaskState::IDLE);
    REQUIRE(t.lower->PopWriteAsHex().empty());

    REQUIRE(t.application->taskStartEvents.size() == 1);
    REQUIRE(t.application->taskStartEvents[0] == MasterTaskType::ASSIGN_CLASS);

    REQUIRE(t.application->taskCompletionEvents.size() == 1);
    REQUIRE(t.application->taskCompletionEvents[0].result == TaskCompletion::SUCCESS);
}

TEST_CASE(SUITE("DisableUnsolBeforeAssignClass"))
{
    auto params = NoStartupTasks();
    params.disableUnsolOnStartup = true;
    MasterTestFixture t(params);

    // configure the mock application to do assign class on startup
    t.application->assignClassHeaders.push_back(Header::AllObjects(60, 2));
    t.application->assignClassHeaders.push_back(Header::AllObjects(3, 0));

    t.context->OnLowerLayerUp();

    REQUIRE(t.exe->run_many() > 0);

    REQUIRE(t.lower->PopWriteAsHex()
            == hex::ClassTask(FunctionCode::DISABLE_UNSOLICITED, 0, ClassField::AllEventClasses()));
}