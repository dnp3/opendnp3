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
package com.automatak.dnp3;

import com.automatak.dnp3.enums.AssignClassType;
import com.automatak.dnp3.enums.PointClass;
import com.automatak.dnp3.enums.RestartMode;

/**
 * Outstation application code implements this interface to interface with the stack
 */
public interface OutstationApplication extends LinkStatusListener {

    /**
     * @return true of the outstation should allow absolute time to be written
     */
    boolean supportsWriteAbsoluteTime();

    /**
     * Called when the master writes the time
     * @param msSinceEpoch milliseconds since Unix epoch
     * @return true, if the wrote is successful
     */
    boolean writeAbsoluteTime(long msSinceEpoch);

    /**
     * @return true if the outstation supports assigning class
     */
    boolean supportsAssignClass();

    /**
     * Called when the master assigns class to a particular type of measurements
     * @param type the type of the assignment
     * @param clazz the class assigned
     * @param start the start of the measurement range
     * @param stop the end of the measurement range
     */
    void recordClassAssignment(AssignClassType type, PointClass clazz, int start, int stop);

    /**
     * @return Application controlled IIN bits
     */
    ApplicationIIN getApplicationIIN();

    /**
     * @return cold restart mode supported
     */
    RestartMode coldRestartSupport();

    /**
     * @return warm restart mode it supported
     */
    RestartMode warmRestartSupport();

    /**
     * The outstation should perform a complete restart. See the DNP3 specification for
     * a complete description of normal behavior
     *
     * @return number of seconds or milliseconds until restart is complete. The value
     * is interpreted based on the Restart Mode returned from ColdRestartSupport()
     */
    int coldRestart();

    /**
     * The outstation should perform a partial restart of only the DNP3 application.
     * See the DNP3 specification for a complete description of normal behavior
     *
     * @return number of seconds or milliseconds until restart is complete. The value
     * is interpreted based on the Restart Mode returned from ColdRestartSupport()
     */
    int warmRestart();

    /**
     * This method notifies that application code that an expected CONFIRM has been
     * received, and events may have cleared from the event buffer. It is informational
     * only.
     *
     * @param isUnsolicited true if the confirm is for an unsolicited response, false for a solicited response
     * @param numClass1 number of Class 1 events remaining in the event buffer after processing the confirm
     * @param numClass2 number of Class 2 events remaining in the event buffer after processing the confirm
     * @param numClass3 number of Class 3 events remaining in the event buffer after processing the confirm
     */
    void onConfirmProcessed(boolean isUnsolicited, long numClass1, long numClass2, long numClass3);

    /**
     * Return the current time and the synchronization status.
     * This value is used when freezing counters
     *
     * @return Current time and timesync status
     */
    DNPTime now();
}
