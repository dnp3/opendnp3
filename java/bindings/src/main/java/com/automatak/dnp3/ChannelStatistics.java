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

public class ChannelStatistics
{
    public ChannelStatistics(
        long numOpen,
        long numOpenFail,
        long numClose,
        long numBytesRx,
        long numBytesTx,
        long numLinkFrameTx
    )
    {
        this.numOpen = numOpen;
        this.numOpenFail = numOpenFail;
        this.numClose = numClose;
        this.numBytesRx = numBytesRx;
        this.numBytesTx = numBytesTx;
        this.numLinkFrameTx = numLinkFrameTx;
    }

    /// The number of times the channel has successfully opened
    public final long numOpen;

    /// The number of times the channel has failed to open
    public final long numOpenFail;

    /// The number of times the channel has closed either due to user intervention or an error
    public final long numClose;

    /// The number of bytes received
    public final long numBytesRx;

    /// The number of bytes transmitted
    public final long numBytesTx;

    /// Number of frames transmitted
    public final long numLinkFrameTx;
}
