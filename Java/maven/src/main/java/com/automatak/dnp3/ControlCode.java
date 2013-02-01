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

public enum ControlCode {
    NULL(0),			    //  illegal command code (used internally)
    PULSE(0x01),		    //  'push-button' interface, can only be pressed one way (reset button on pedometer)
    LATCH_ON(0x03),		//  'light-switch' moved to the ON position
    LATCH_OFF(0x04),	    //  'light-switch' moved to the OFF position
    PULSE_CLOSE(0x41),	//  'doorbell' that rings while the button is depressed
    PULSE_TRIP(0x81),	//  'doorbell' that stops ringing (is normally on) while depreseed
    UNDEFINED(0xFF);		//   undefined command (used by DNP standard)

    private final int id;

    private ControlCode(int id)
    {
        this.id = id;
    }

    public int toInt()
    {
        return id;
    }

    public static ControlCode fromInt(int id)
    {
        switch(id)
        {
            case(0):
                return NULL;
            case(0x01):
                return PULSE;
            case(0x03):
                return LATCH_ON;
            case(0x04):
                return LATCH_OFF;
            case(0x41):
                return PULSE_CLOSE;
            case(0x81):
                return PULSE_TRIP;
            default:
                return UNDEFINED;
        }
    }
};
