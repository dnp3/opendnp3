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

import com.automatak.dnp3.enums.ServerAcceptMode;

import java.util.List;

/**
 * The main entry point for all dnp3 applications. Use this object to create communication channels to which
 * you can then add masters and outstations.
 *
 * To obtain a concrete implementation of this interface, use the DNP3ManagerFactory class in the impl package.
 *
 */
public interface DNP3Manager {

    /**
     * Add a TCP client channel. The channel does not try to connect until you add a stack.
     * @param id An id used for logging purposes
     * @param levels The starting level for logging output
     * @param retry Retry configuration for the channel
     * @param remotes List of IP endpoints to try to connect to. The address can be an IP "127.0.0.1" or name "www.google.com"
     * @param adapter The local adapter to use. "0.0.0.0* means "any adapter'.
     * @param listener Optional listener (can be null) for monitoring the state of the channel
     * @return A channel interface
     * @throws DNP3Exception if an error occurs creating the channel
     */
    Channel addTCPClient(String id, int levels, ChannelRetry retry, List<IPEndpoint> remotes, String adapter, ChannelListener listener) throws DNP3Exception;

    /**
     * Add a TCP client channel. The channel does not try to connect until you add a stack.
     * @param id An id used for logging purposes
     * @param levels The starting level for logging output
     * @param mode Describes which TCP session is closed when an active session already exists
     * @param endpoint The address that identifies the network adapter to bind (i.e. "127.0.0.1" or "0.0.0.0") and the port
     * @param listener Optional listener (can be null) for monitoring the state of the channel
     * @return A channel interface
     * @throws DNP3Exception if an error occurs creating the channel
     */
    Channel addTCPServer(String id, int levels, ServerAcceptMode mode, IPEndpoint endpoint, ChannelListener listener) throws DNP3Exception;

    /**
     * Add a UDP channel.
     * @param id An id used for logging purposes
     * @param levels The starting level for logging output
     * @param retry Retry configuration for the channel
     * @param localEndpoint Local endpoint from which datagrams will be received
     * @param remoteEndpoint Remote endpoint where datagrams will be sent to
     * @param listener Optional listener (can be null) for monitoring the state of the channel
     * @return A channel interface
     * @throws DNP3Exception if an error occurs creating the channel
     */
    Channel addUDPChannel(String id, int levels, ChannelRetry retry, IPEndpoint localEndpoint, IPEndpoint remoteEndpoint, ChannelListener listener) throws DNP3Exception;

    /**
     * Add a TCP client channel. The channel does not try to connect until you add a stack.
     * @param id An id used for logging purposes
     * @param levels The starting level for logging output
     * @param retry Retry configuration for the channel
     * @param remotes List of IP endpoints to try to connect to. The address can be an IP "127.0.0.1" or name "www.google.com"
     * @param adapter The local adapter to use. "0.0.0.0* means "any adapter'.
     * @param config TLS configuration
     * @param listener Optional listener (can be null) for monitoring the state of the channel
     * @return A channel interface
     * @throws DNP3Exception if an error occurs creating the channel
     */
    Channel addTLSClient(String id, int levels, ChannelRetry retry, List<IPEndpoint> remotes, String adapter, TLSConfig config, ChannelListener listener) throws DNP3Exception;

    /**
     * Add a TCP client channel. The channel does not try to connect until you add a stack.
     * @param id An id used for logging purposes
     * @param levels The starting level for logging output
     * @param mode Describes which TCP session is closed when an active session already exists
     * @param endpoint The address that identifies the network adapter to bind (i.e. "127.0.0.1" or "0.0.0.0") and port
     * @param config TLS configuration
     * @param listener Optional listener (can be null) for monitoring the state of the channel
     * @return A channel interface
     * @throws DNP3Exception if an error occurs creating the channel
     */
    Channel addTLSServer(String id, int levels, ServerAcceptMode mode, IPEndpoint endpoint, TLSConfig config, ChannelListener listener) throws DNP3Exception;

    /**
     * Add a serial channel. The port does not try to open until you add a stack.
     *
     * @param id An id used for logging purposes
     * @param levels The starting level for logging output
     * @param retry Retry configuration for the channel
     * @param settings Configuration for the serial port
     * @param listener Optional listener (can be null) for monitoring the state of the channel
     * @throws DNP3Exception if an error occurs creating the channel
     * @return a channel interface
     */
    Channel addSerial(String id, int levels, ChannelRetry retry, SerialSettings settings, ChannelListener listener) throws DNP3Exception;


    /**
     * Permanently shutdown all channels and any associated stacks
     */
    void shutdown();
}
