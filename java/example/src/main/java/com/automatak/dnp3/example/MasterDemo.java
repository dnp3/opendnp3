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
import com.automatak.dnp3.enums.CommandStatus;
import com.automatak.dnp3.enums.ControlCode;
import com.automatak.dnp3.impl.DNP3ManagerFactory;
import com.automatak.dnp3.mock.DefaultMasterApplication;
import com.automatak.dnp3.mock.PrintingLogHandler;
import com.automatak.dnp3.mock.PrintingSOEHandler;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.concurrent.CompletableFuture;

/**
 * Example master than can be run against the example outstation
 */
public class MasterDemo {

    public static void main(String[] args) throws Exception {

        // create the root class with a thread pool size of 1
        DNP3Manager manager = DNP3ManagerFactory.createManager(1, PrintingLogHandler.getInstance());

        /*
        // Create a tcp channel class that will connect to the loopback
        Channel channel = manager.addTCPClient("client", LogMasks.NORMAL, ChannelRetry.getDefault(), "127.0.0.1", 20000);

        // You can modify the defaults to change the way the master behaves
        MasterStackConfig config = new MasterStackConfig();

        // Create a master instance, pass in a simple singleton to print received values to the console
        Master master = channel.addMaster("master", PrintingSOEHandler.getInstance(), DefaultMasterApplication.getInstance(), config);

        // This sub-interface can issue command requests
        CommandProcessor processor = master.getCommandProcessor();

        // all this cruft just to read a line of text in Java. Oh the humanity.
        InputStreamReader converter = new InputStreamReader(System.in);
        BufferedReader in = new BufferedReader(converter);

        while (true) {
            System.out.println("Enter something to issue a command or type <quit> to exit");
            String line = in.readLine();
            if(line.equals("quit")) break;
            else {
                ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.LATCH_ON, (short) 1, 100, 100, CommandStatus.SUCCESS);
                CompletableFuture<CommandTaskResult> future = processor.selectAndOperate(crob, 0, TaskConfig.getDefault());
                System.out.println("Command result: " + future.get().toString());
            }
        }
        */

        // This call is needed b/c the thread-pool will stop the application from exiting
        // Also, the finalizer isn't guaranteed to run.
        manager.shutdown();

    }

}
