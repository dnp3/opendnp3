/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
package com.automatak.dnp3.impl;

import com.automatak.dnp3.DNP3Manager;
import com.automatak.dnp3.impl.mocks.NullLogHandler;
import com.automatak.dnp3.impl.mocks.StackPair;
import junit.framework.TestCase;
import org.junit.Test;

import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

public class DNP3ManagerIntegrationTest extends TestCase {

    static final int NUM_STACKS = 10;
    static final int NUM_POINTS_PER_EVENT_TYPE = 50;
    static final int NUM_ITERATIONS = 10;
    static final int EVENTS_PER_ITERATION = 50;
    static final int START_PORT = 20010;

    static final int NUM_THREADS_IN_POOL = 4;
    static final Duration TIMEOUT = Duration.ofSeconds(10);

    static void withManager(int numThreads, Consumer<DNP3Manager> func)
    {
        final DNP3Manager manager = DNP3ManagerFactory.createManager(numThreads, new NullLogHandler());
        try {
            func.accept(manager);
        }
        finally {
            manager.shutdown();
        }
    }
    
    @Test
    public void testEventOrdering() {

        List<StackPair> stacks = new ArrayList<>();

        withManager(NUM_THREADS_IN_POOL, manager ->  {

            for(int i = 0; i < NUM_STACKS; ++i) {
                StackPair pair = new StackPair(manager, START_PORT+i, NUM_POINTS_PER_EVENT_TYPE, EVENTS_PER_ITERATION);
                stacks.add(pair);
            }

            final long start = System.currentTimeMillis();

            stacks.forEach(pair -> pair.waitForChannelsOpen(TIMEOUT));

            for(int i = 0; i < NUM_ITERATIONS; ++i) {

                stacks.forEach(pair -> pair.sendRandomValues());
                stacks.forEach(pair -> pair.awaitSentValues(TIMEOUT));
            }

            final long ELASPED_MS = System.currentTimeMillis() - start;
            final long TOTAL_EVENTS = NUM_STACKS*NUM_ITERATIONS*EVENTS_PER_ITERATION;
            final long RATE = (TOTAL_EVENTS * 1000)/ ELASPED_MS;

            System.out.println(String.format("%d events in %d ms == %d events/sec", TOTAL_EVENTS, ELASPED_MS, RATE));
        });

    }

    @Test
    public void testCommandOrdering() {

        List<StackPair> stacks = new ArrayList<>();

        withManager(NUM_THREADS_IN_POOL, manager ->  {

            for(int i = 0; i < NUM_STACKS; ++i) {
                StackPair pair = new StackPair(manager, START_PORT+i, NUM_POINTS_PER_EVENT_TYPE, EVENTS_PER_ITERATION);
                stacks.add(pair);
            }

            final long start = System.currentTimeMillis();

            stacks.forEach(pair -> pair.waitForChannelsOpen(TIMEOUT));

            for(int i = 0; i < NUM_ITERATIONS; ++i) {

                stacks.forEach(pair -> pair.sendRandomCommands());
                stacks.forEach(pair -> pair.awaitSentCommands(TIMEOUT));
            }

            final long ELASPED_MS = System.currentTimeMillis() - start;
            final long TOTAL_EVENTS = NUM_STACKS*NUM_ITERATIONS*EVENTS_PER_ITERATION;
            final long RATE = (TOTAL_EVENTS * 1000)/ ELASPED_MS;

            System.out.println(String.format("%d commands in %d ms == %d commands/sec", TOTAL_EVENTS, ELASPED_MS, RATE));
        });

    }

}
