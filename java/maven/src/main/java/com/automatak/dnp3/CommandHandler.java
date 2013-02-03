/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3;

/**
 * Outstation application code implements this interface to handle command requests from a master
 */
public interface CommandHandler {

    /**
     * Select a ControlRelayOutputBlock (Group12Var1)
     * @param command command object
     * @param index request index
     * @param sequence application sequence number
     * @return Enumeration representing the result of the request
     */
    CommandStatus Select(ControlRelayOutputBlock command, long index, byte sequence);

    /**
     * Select a 32-bit integer AnalogOutput (Group41Var1)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus Select(AnalogOutputInt32 command, long index, byte sequence);

    /**
     * Select a 16-bit integer AnalogOutput (Group41Var2)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus Select(AnalogOutputInt16 command, long index, byte sequence);

    /**
     * Select a single precision AnalogOutput (Group41Var3)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus Select(AnalogOutputFloat32 command, long index, byte sequence);

    /**
     * Select a double precision AnalogOutput (Group41Var4)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus Select(AnalogOutputDouble64 command, long index, byte sequence);

    /**
     * Operate a ControlRelayOutputBlock (Group12Var1)
     * @param command command object
     * @param index request index
     * @param sequence application sequence number
     * @return Enumeration representing the result of the request
     */
    CommandStatus Operate(ControlRelayOutputBlock command, long index, byte sequence);

    /**
     * Operate a 32-bit integer AnalogOutput (Group41Var1)
     * @param command command object
     * @param index request index
     * @param sequence application sequence number
     * @return Enumeration representing the result of the request
     */
    CommandStatus Operate(AnalogOutputInt32 command, long index, byte sequence);

    /**
     * Operate a 16-bit integer AnalogOutput (Group41Var2)
     * @param command command object
     * @param index request index
     * @param sequence application sequence number
     * @return Enumeration representing the result of the request
     */
    CommandStatus Operate(AnalogOutputInt16 command, long index, byte sequence);

    /**
     * Operate a single precision AnalogOutput (Group41Var3)
     * @param command command object
     * @param index request index
     * @param sequence application sequence number
     * @return Enumeration representing the result of the request
     */
    CommandStatus Operate(AnalogOutputFloat32 command, long index, byte sequence);

    /**
     * Operate a double precision AnalogOutput (Group41Var4)
     * @param command command object
     * @param index request index
     * @param sequence application sequence number
     * @return Enumeration representing the result of the request
     */
    CommandStatus Operate(AnalogOutputDouble64 command, long index, byte sequence);

    /**
     * DirectOperate a ControlRelayOutputBlock (Group12Var1)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus DirectOperate(ControlRelayOutputBlock command, long index);

    /**
     * DirectOperate a 32-bit integer AnalogOutput (Group41Var1)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus DirectOperate(AnalogOutputInt32 command, long index);

    /**
     * DirectOperate a 16-bit integer AnalogOutput (Group41Var2)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus DirectOperate(AnalogOutputInt16 command, long index);

    /**
     * DirectOperate a single precision AnalogOutput (Group41Var3)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus DirectOperate(AnalogOutputFloat32 command, long index);

    /**
     * DirectOperate a double precision AnalogOutput (Group41Var4)
     * @param command command object
     * @param index request index
     * @return Enumeration representing the result of the request
     */
    CommandStatus DirectOperate(AnalogOutputDouble64 command, long index);

}
