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
 * A communication channel to which DNP3 masters / outstation can be attached
 */
public interface Channel {

    /**
     * Shutdown the channel and all stacks that have been added. Calling shutdown more than once or
     * continuing to use child objects (masters/outstations) after calling shutdown can cause a failure.
     */
    void shutdown();

    /**
     * Add a listener to receive state change messages. All callbacks come from the underlying thread pool.
     * Listeners can be bound at any time and you will always receive an immediate callback with the current
     * state.
     *
     * @param listener Callback interface to receive the messages
     */
    void addStateListener(ChannelStateListener listener);

    /**
     * Adds a master to the channel
     * @param loggerId name of the logger that will be assigned to this stack
     * @param level LogLevel assigned to the logger
     * @param publisher Where measurements will be sent as they are received from the outstation
     * @param config configuration information for the master stack
     * @return reference to the created master
     */
    Master addMaster(String loggerId, LogLevel level, DataObserver publisher, MasterStackConfig config);

    /**
     * Adds an outstation to the channel
     * @param loggerId name of the logger that will be assigned to this stack
     * @param level LogLevel assigned to the logger
     * @param cmdHandler where command requests are sent to be handled in application code
     * @param config configuration information for the outstation stack
     * @return reference to the created master
     */
    Outstation addOutstation(String loggerId, LogLevel level, CommandHandler cmdHandler, OutstationStackConfig config);
}
