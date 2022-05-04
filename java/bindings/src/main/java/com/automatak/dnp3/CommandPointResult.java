/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
package com.automatak.dnp3;

import com.automatak.dnp3.enums.CommandPointState;
import com.automatak.dnp3.enums.CommandStatus;

/**
 * Aggregate result of command operation.  Check the Result before checking the status.
 * Status is only valid when Result == RESPONSE_OK
 */
public class CommandPointResult
{
    public CommandPointResult(int headerIndex, int requestIndex, CommandPointState state, CommandStatus status)
    {
        this.headerIndex = headerIndex;
        this.requestIndex = requestIndex;
        this.state = state;
        this.status = status;
    }

    @Override
    public String toString()
    {
        return String.format("Result(header: %d index: %d State: %s Status: %s)", headerIndex, requestIndex, state, status);
    }


    public final int headerIndex;
    public final int requestIndex;
    public final CommandPointState state;
    public final CommandStatus status;
}
