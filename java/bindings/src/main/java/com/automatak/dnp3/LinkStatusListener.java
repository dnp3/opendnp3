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

import com.automatak.dnp3.enums.LinkStatus;

/**
 * Various informational callbacks that can be received from the link layer
 */
public interface LinkStatusListener
{
    /**
     * Called when a the reset/unreset status of the link layer changes
     * @param value the current state of the link layer
     */
    default void onStateChange(LinkStatus value) {}

    /**
     * Called when a link-layer frame is received from an unknown destination address
     */
    default void onUnknownDestinationAddress(int destination) {}

    /**
     * Called when a link-layer frame is received from an unknown source address
     */
    default void onUnknownSourceAddress(int source) {}

    /**
     * Called when a keep alive message is transmitted
     */
    default void onKeepAliveInitiated() {}

    /**
     * Called when a keep alive message (request link status) receives no response
     */
    default void onKeepAliveFailure() {}

    /**
     * Called when a keep alive message receives a valid response
     */
    default void onKeepAliveSuccess() {}
}
