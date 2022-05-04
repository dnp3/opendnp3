/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements. 
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

import java.time.Duration;

/**
 * Configuration class for the link layer
 */
public class LinkLayerConfig {

    /**
     * Controls the master/slave bit set on all messages
     */
    public boolean isMaster;

    /**
     * dnp3 address of the local device as a 16-bit unsigned integer
     */
    public int localAddr;

    /**
     * dnp3 address of the remote device as a 16-bit unsigned integer
     */
    public int remoteAddr;

    /**
     * the response timeout for confirmed requests and link status requests
     */
    public Duration responseTimeout = Duration.ofSeconds(1);

    /**
     * the keep-alive timer timeout interval
     */
    public Duration keepAliveTimeout = Duration.ofSeconds(60);

    private LinkLayerConfig()
    {

    }

    /**
     * Constructor that sets reasonable defaults. Override after construction
     * @param isMaster True if the link layer is for a master, false for an outstation
     */
    public LinkLayerConfig(boolean isMaster)
    {
        this.isMaster = isMaster;
        this.localAddr = isMaster ? 1 : 1024;
        this.remoteAddr = isMaster ? 1024: 1;
    }

}
