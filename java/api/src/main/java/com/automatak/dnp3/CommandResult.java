/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3;

/**
 * Enumeration that represents that result of command operation on an outstation
 */
public enum CommandResult {

    /**
     * A response was received from the outstation, check the CommandStatus enumeration
     */
    RESPONSE_OK(0),

    /**
     * The operation timed out without a response or failed after a select
     */
    TIMEOUT(1),

    /**
     * There is no communication with the outstation, and the command was not attempted
     */
    NO_COMMS(2);


    private final int id;

    private CommandResult(int id)
    {
        this.id = id;
    }

    public int toInt()
    {
        return id;
    }

    public static CommandResult fromInt(int id)
    {
        switch(id)
        {
            case(0): return RESPONSE_OK;
            case(1): return TIMEOUT;
            case(2): return NO_COMMS;

            default: return NO_COMMS;
        }
    }
};

