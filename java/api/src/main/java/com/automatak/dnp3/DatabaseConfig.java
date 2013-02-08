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

import java.util.ArrayList;
import java.util.List;

/**
 * Configuration class for the outstation database
 */
public class DatabaseConfig {

    public final List<EventPointRecord> binaryInputs;
    public final List<DeadbandPointRecord> analogInputs;
    public final List<EventPointRecord> counterInputs;
    public final List<PointRecord> binaryOutputStatii;
    public final List<PointRecord> analogOutputStatii;

    /**
     * Construct a default database with given size. Then override settings for individual points.
     * @param numBinary number of binary input point
     * @param numAnalog number of analog input points
     * @param numCounter number of counter points
     * @param numBinaryOutputStatus number of binary output status points
     * @param numAnalogOutputStatus number of analog output status points
     */
    public DatabaseConfig(int numBinary, int numAnalog, int numCounter, int numBinaryOutputStatus, int numAnalogOutputStatus)
    {
        this.binaryInputs = new ArrayList<EventPointRecord>(numBinary);
        for(int i=0; i<numBinary; ++i) this.binaryInputs.add(new EventPointRecord());

        this.analogInputs = new ArrayList<DeadbandPointRecord>(numAnalog);
        for(int i=0; i<numAnalog; ++i) this.analogInputs.add(new DeadbandPointRecord());

        this.counterInputs = new ArrayList<EventPointRecord>(numCounter);
        for(int i=0; i<numCounter; ++i) this.counterInputs.add(new EventPointRecord());

        this.binaryOutputStatii = new ArrayList<PointRecord>(numBinaryOutputStatus);
        for(int i=0; i<numBinaryOutputStatus; ++i) this.binaryOutputStatii.add(new PointRecord());

        this.analogOutputStatii = new ArrayList<PointRecord>(numAnalogOutputStatus);
        for(int i=0; i<numAnalogOutputStatus; ++i) this.analogOutputStatii.add(new PointRecord());

    }

}
