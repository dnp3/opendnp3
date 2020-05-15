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

import java.time.Duration;

/**
 * Interface representing a master station
 */
public interface Master extends Stack, CommandProcessor {


    /**
     * Perform a one time scan with the supplied headers
     * @param headers The request headers for the READ
     * @param soeHandler The handler of the response
     */
    void scan(Iterable<Header> headers, SOEHandler soeHandler);

    /**
     * Add a periodic scan with the supplied headers
     * @param period The period at which to perform the scan
     * @param headers The request headers for the READ
     * @param soeHandler The handler of the response
     */
    void addPeriodicScan(Duration period, Iterable<Header> headers, SOEHandler soeHandler);


    /**
     * Perform a one time scan with the supplied headers
     * @param header The request header for the READ
     * @param soeHandler The handler of the response
     */
    void scan(Header header, SOEHandler soeHandler);

    /**
     * Add a periodic scan with the supplied headers
     * @param period The period at which to perform the scan
     * @param header The request header for the READ
     * @param soeHandler The handler of the response
     */
    void addPeriodicScan(Duration period, Header header, SOEHandler soeHandler);

}
