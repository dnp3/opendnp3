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

public class OutstationStackConfig {

    /**
     * Link layer configuration
     */
    public final LinkLayerConfig linkConfig = new LinkLayerConfig(false);

    /**
     * Configuration for the outstation (everything but the database layout)
     */
    public final OutstationConfig outstationConfig = new OutstationConfig();

    /**
     * Outstation database configuration
     */
    public final DatabaseConfig databaseConfig;

    /**
     * Maximum events buffered for each type
     */
    public final EventBufferConfig eventBufferConfig;

    /**
     * Constructor for an outstation stack with sensible defaults
     * @param databaseConfig Database layout for the outstation
     * @param eventBufferConfig Configuration of the event buffer
     */
    public OutstationStackConfig(DatabaseConfig databaseConfig, EventBufferConfig eventBufferConfig)
    {
        this.databaseConfig = databaseConfig;
        this.eventBufferConfig = eventBufferConfig;
    }
}
