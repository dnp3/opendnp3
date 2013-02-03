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
package com.automatak.dnp3.example;

import com.automatak.dnp3.*;

public class ExampleMaster {

    public static void main(String[] args) throws InterruptedException
    {
        DNP3Manager mgr = DNP3ManagerFactory.createManager(4);

        mgr.addLogSubscriber(new PrintingLogSubscriber());

        Channel server = mgr.addTCPServer("server", LogLevel.INFO, 5000, "127.0.0.1", 20000);
        Outstation outstation = server.addOutstation("outstation", LogLevel.INFO, new ConstantCommandHandler(CommandStatus.HARDWARE_ERROR));
        Channel client = mgr.addTCPClient("client", LogLevel.INFO, 5000, "127.0.0.1", 20000);
        Master master = client.addMaster("master", LogLevel.INFO, new PrintingDataObserver());

        Thread.sleep(1000);

        DataObserver obs = outstation.getDataObserver();

        obs.start();
        obs.update(new BinaryInput(true, (byte) 1, 0), 0);
        obs.update(new AnalogInput(23.6, (byte) 1, 0), 0);
        obs.end();

        Thread.sleep(10000);

        mgr.shutdown();

    }
}
