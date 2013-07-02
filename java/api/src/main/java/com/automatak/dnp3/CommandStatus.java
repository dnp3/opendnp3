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
public enum CommandStatus {

    /**
     * command was successfully received and handled
     */
    SUCCESS(0),

    /**
     * command timeout before completing
     */
    TIMEOUT(1),

    /**
     * command requires being selected before operate, configuration issue
     */
    NO_SELECT(2),

    /**
     * bad control code or timing values
     */
    FORMAT_ERROR(3),

    /**
     * command is not implemented
     */
    NOT_SUPPORTED(4),

    /**
     * command is already in progress or its already in that mode
     */
    ALREADY_ACTIVE(5),

    /**
     * something is stopping the command, often a local/remote interlock
     */
    HARDWARE_ERROR(6),

    /**
     * the function governed by the control is in local only control
     */
    LOCAL(7),

    /**
     * the command has been done too often and has been throttled
     */
    TOO_MANY_OPS (8),

    /**
     * the command was rejected because the device denied it or an RTU intercepted it
     */
    NOT_AUTHORIZED(9),

    /**
     *  10 to 126 are currently reserved
     */
    UNDEFINED(127);

    private final int id;

    private CommandStatus(int id)
    {
        this.id = id;
    }

    public int toInt()
    {
        return id;
    }

    public static CommandStatus fromInt(int id)
    {
        switch(id)
        {
            case(0): return SUCCESS;
            case(1): return TIMEOUT;
            case(2): return NO_SELECT;
            case(3): return FORMAT_ERROR;
            case(4): return NOT_SUPPORTED;
            case(5): return ALREADY_ACTIVE;
            case(6): return HARDWARE_ERROR;
            case(7): return LOCAL;
            case(8): return TOO_MANY_OPS;
            case(9): return NOT_AUTHORIZED;
            default: return UNDEFINED;
        }
    }
};

