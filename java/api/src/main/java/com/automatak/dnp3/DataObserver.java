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
 * Interface called to load data into an outstation or receive data from a master
 *
 * start() / end() must be called before / after any calls to update.
 */
public interface DataObserver {

    /*
     * Start a transaction, end() must be called to release the resource
     */
    void start();

    /**
     * Update a BinaryInput measurement
     * @param meas measurement object
     * @param index index of measurement
     */
    void update(BinaryInput meas, long index);

    /**
     * Update an AnalogInput measurement
     * @param meas measurement object
     * @param index index of measurement
     */
    void update(AnalogInput meas, long index);

    /**
     * Update a Counter measurement
     * @param meas measurement object
     * @param index index of measurement
     */
    void update(Counter meas, long index);

    /**
     * Update a BinaryOutputStatus measurement
     * @param meas measurement object
     * @param index index of measurement
     */
    void update(BinaryOutputStatus meas, long index);

    /**
     * Update a AnalogOutputStatus measurement
     * @param meas measurement object
     * @param index index of measurement
     */
    void update(AnalogOutputStatus meas, long index);

    /*
     * End a transaction
     */
    void end();

}
