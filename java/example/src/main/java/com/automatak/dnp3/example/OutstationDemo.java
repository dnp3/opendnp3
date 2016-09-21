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
package com.automatak.dnp3.example;

import com.automatak.dnp3.*;
import com.automatak.dnp3.impl.DNP3ManagerFactory;
import com.automatak.dnp3.mock.PrintingLogSubscriber;
import com.automatak.dnp3.mock.SuccessCommandHandler;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Example master than can be run against the example outstation
 */
public class OutstationDemo {

    public static void main(String[] args) throws IOException, InterruptedException {

        // create the root class with a thread pool size of 1
        DNP3Manager manager = DNP3ManagerFactory.createManager(1);

        // You can send the log messages anywhere you want
        // but PrintingLogSubscriber just prints them to the console
        manager.addLogSubscriber(PrintingLogSubscriber.getInstance());

        // Create a tcp channel class that will connect to the loopback
        Channel channel = manager.addTCPServer("client", LogLevel.INFO, 5000, "127.0.0.1", 20000);

        // You can optionally add a listener to receive state changes on the channel
        channel.addStateListener(new ChannelStateListener() {
            @Override
            public void onStateChange(ChannelState state) {
                System.out.println("server state: " + state);
            }
        });

        // Outstation will have 5 of every measurement type
        DatabaseConfig db = new DatabaseConfig(5,5,5,5,5);

        // Create the default outstation configuration
        OutstationStackConfig config = new OutstationStackConfig(db);
        config.outstationConfig.staticAnalogInput = StaticAnalogResponse.GROUP30_VAR1;

        // Create an Outstation instance, pass in a simple a command handler that responds successfully to everything
        Outstation outstation = channel.addOutstation("outstation", LogLevel.INTERPRET, SuccessCommandHandler.getInstance(), config);

        // You can optionally add a listener to receive state changes on the stack
        outstation.addStateListener(new StackStateListener() {
            @Override
            public void onStateChange(StackState state) {
                System.out.println("Outstation state: " + state);
            }
        });

        // This sub-interface allows us to load data into the outstation
        DataObserver data = outstation.getDataObserver();

        // all this stuff just to read a line of text in Java. Oh the humanity.
        String line = "";
        InputStreamReader converter = new InputStreamReader(System.in);
        BufferedReader in = new BufferedReader(converter);


        int i = 0;
        while (true) {
            System.out.println("Enter something to update a counter or type <quit> to exit");
            line = in.readLine();
            if(line.equals("quit")) break;
            else {
                data.start();
                data.update(new Counter(i, CounterInputQuality.ONLINE.toByte(), 0), 0);
                data.end();
                ++i;
            }
        }

        manager.shutdown();
    }
}
