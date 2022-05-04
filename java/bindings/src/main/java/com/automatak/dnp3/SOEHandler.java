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

/**
 * Interface called to process measurement data received from an outstation
 *
 * start() / end() will be called before / after any calls to update.
 */
public interface SOEHandler {

    /**
     * Start a processing an ASDU
     * @param info Information about the incoming ASDU
     */
    void beginFragment(ResponseInfo info);

    /**
     * End a processing an ASDU
     * @param info Information about the incoming ASDU
     */
    void endFragment(ResponseInfo info);

    /**
     * Process a collection of values
     * @param info information about the header from which the value came
     * @param values the measurement values
     */
    void processBI(HeaderInfo info, Iterable<IndexedValue<BinaryInput>> values);

    /**
     * Process a collection of values
     * @param info information about the header from which the value came
     * @param values the measurement values
     */
    void processDBI(HeaderInfo info, Iterable<IndexedValue<DoubleBitBinaryInput>> values);

    /**
     * Process a collection of values
     * @param info information about the header from which the value came
     * @param values the measurement values
     */
    void processAI(HeaderInfo info, Iterable<IndexedValue<AnalogInput>> values);

    /**
     * Process a collection of values
     * @param info information about the header from which the value came
     * @param values the measurement value
     */
    void processC(HeaderInfo info, Iterable<IndexedValue<Counter>> values);

    /**
     * Process a collection of values
     * @param info information about the header from which the value came
     * @param values the measurement values
     */
    void processFC(HeaderInfo info, Iterable<IndexedValue<FrozenCounter>> values);

    /**
     * Process a collection of values
     * @param info information about the header from which the value came
     * @param values the measurement values
     */
    void processBOS(HeaderInfo info, Iterable<IndexedValue<BinaryOutputStatus>> values);

    /**
     * Process a collection of values
     * @param info information about the header from which the value came
     * @param values the measurement values
     */
    void processAOS(HeaderInfo info, Iterable<IndexedValue<AnalogOutputStatus>> values);

    /**
     * Process a collection of values
     * @param info information about the header from which the value came
     * @param values the measurement values
     */
    void processDNPTime(HeaderInfo info, Iterable<DNPTime> values);

}
