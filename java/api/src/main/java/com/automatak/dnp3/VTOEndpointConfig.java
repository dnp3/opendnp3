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

/**
 * Configuration for a VTO endpoint, independent of client / server.
 */
public class VTOEndpointConfig {

    public final byte channelId;
    public final long minOpenRetryMs;
    public final long maxOpenRetryMs;
    public final boolean startLocal;
    public final boolean disableExtenstion;

    /**
     * Fully parameterized constructor with no defaults
     * @param channelId     Each dnp index for Vto data events is a channel id
     * @param startLocal    If true, always try to keep the local connection online, otherwise we only connect when the remote side connects
     * @param disableExtensions  If true, the router defaults to the VTO specification and does not publish/utilize the connection state information
     * @param minOpenRetryMs  minimum time to wait before retrying opening the physical layer after a failure, doubles up until maximum
     * @param maxOpenRetryMs  maximum time to wait before retrying opening the physical layer after a failure
     */
    public VTOEndpointConfig(byte channelId, boolean startLocal, boolean disableExtensions, long minOpenRetryMs, long maxOpenRetryMs)
    {
        this.channelId = channelId;
        this.startLocal = startLocal;
        this.disableExtenstion = disableExtensions;
        this.minOpenRetryMs = minOpenRetryMs;
        this.maxOpenRetryMs = maxOpenRetryMs;
    }

    /**
     * Constructor that defaults timing to 5s and 10s
     * @param channelId  Each dnp index for Vto data events is a channel id
     * @param startLocal If true, always try to keep the local connection online, otherwise we only connect when the remote side connects
     * @param disableExtenstions If true, the router defaults to the VTO specification and does not publish/utilize the connection state information
     */
    public VTOEndpointConfig(byte channelId, boolean startLocal, boolean disableExtenstions)
    {
       this(channelId, startLocal, disableExtenstions, 5000, 10000);
    }
}
