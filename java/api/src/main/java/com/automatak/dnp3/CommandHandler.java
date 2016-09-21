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

import com.automatak.dnp3.enums.CommandStatus;

/**
 * Outstation application code implements this interface to handle command requests from a master
 */
public interface CommandHandler {

    /**
     * Select a ControlRelayOutputBlock (Group12Var1)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus select(ControlRelayOutputBlock command, long index);

    /**
     * Select a 32-bit integer AnalogOutput (Group41Var1)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus select(AnalogOutputInt32 command, long index);

    /**
     * Select a 16-bit integer AnalogOutput (Group41Var2)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus select(AnalogOutputInt16 command, long index);

    /**
     * Select a single precision AnalogOutput (Group41Var3)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus select(AnalogOutputFloat32 command, long index);

    /**
     * Select a double precision AnalogOutput (Group41Var4)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus select(AnalogOutputDouble64 command, long index);

    /**
     * Operate a ControlRelayOutputBlock (Group12Var1)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus operate(ControlRelayOutputBlock command, long index);

    /**
     * Operate a 32-bit integer AnalogOutput (Group41Var1)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus operate(AnalogOutputInt32 command, long index);

    /**
     * Operate a 16-bit integer AnalogOutput (Group41Var2)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus operate(AnalogOutputInt16 command, long index);

    /**
     * Operate a single precision AnalogOutput (Group41Var3)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus operate(AnalogOutputFloat32 command, long index);

    /**
     * Operate a double precision AnalogOutput (Group41Var4)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus operate(AnalogOutputDouble64 command, long index);

    /**
     * DirectOperate a ControlRelayOutputBlock (Group12Var1)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus directOperate(ControlRelayOutputBlock command, long index);

    /**
     * DirectOperate a 32-bit integer AnalogOutput (Group41Var1)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus directOperate(AnalogOutputInt32 command, long index);

    /**
     * DirectOperate a 16-bit integer AnalogOutput (Group41Var2)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus directOperate(AnalogOutputInt16 command, long index);

    /**
     * DirectOperate a single precision AnalogOutput (Group41Var3)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus directOperate(AnalogOutputFloat32 command, long index);

    /**
     * DirectOperate a double precision AnalogOutput (Group41Var4)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus directOperate(AnalogOutputDouble64 command, long index);

}
