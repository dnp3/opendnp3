/**
 * Copyright 2013-2016 Automatak, LLC
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
package com.automatak.dnp3.impl;

import com.automatak.dnp3.DNP3Exception;
import com.automatak.dnp3.DNP3Manager;
import com.automatak.dnp3.impl.mocks.NullLogHandler;
import com.automatak.dnp3.impl.mocks.StackPair;
import com.automatak.dnp3.mock.PrintingLogHandler;
import junit.framework.*;


import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

public class DNP3ManagerIntegrationTest extends TestCase {

    static final int NUM_STACKS = 100;
    static final int START_PORT = 20000;


    static void withManager(Consumer<DNP3Manager> func)
    {
        DNP3Manager manager= null;
        try {
            manager = DNP3ManagerFactory.createManager(new NullLogHandler());
            func.accept(manager);
        }
        catch(DNP3Exception ex)
        {

        }
        finally {
            if(manager != null) {
                manager.shutdown();
            }

        }
    }

    public void testEventOrdering() {

        List<StackPair> stacks = new ArrayList<>();

        withManager(manager ->  {

            for(int i = 0; i < NUM_STACKS; ++i) {
                StackPair pair = new StackPair(manager, START_PORT+i);
                stacks.add(pair);
            }
        });

    }
}
