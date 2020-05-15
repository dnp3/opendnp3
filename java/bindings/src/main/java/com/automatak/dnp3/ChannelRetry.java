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
 * Constrains the time range used for exponential backoff from some minimum to a maximum
 */
public class ChannelRetry
{
    /**
     * Construct a channel retry class
     * @param minRetryDelay minimum retry delay
     * @param maxRetryDelay maximum retry delay
     * @param reconnectDelay delay between reconnections when a read/write fails
     */
    public ChannelRetry(Duration minRetryDelay, Duration maxRetryDelay, Duration reconnectDelay)
    {
        this.minRetryDelay = minRetryDelay;
        this.maxRetryDelay = maxRetryDelay;
        this.reconnectDelay = reconnectDelay;
    }

    /// <summary>
    /// Default configuration from 1 second to 1 minute
    /// </summary>
    public static ChannelRetry getDefault()
    {
        return new ChannelRetry(Duration.ofSeconds(1), Duration.ofMinutes(1), Duration.ofSeconds(0));
    }

    public final Duration minRetryDelay;
    public final Duration maxRetryDelay;
    public final Duration reconnectDelay;
}
