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
package com.automatak.dnp3.impl.mocks;


import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.*;
import com.automatak.dnp3.mock.DefaultMasterApplication;
import com.automatak.dnp3.mock.DefaultOutstationApplication;
import com.automatak.dnp3.mock.SuccessCommandHandler;

import java.time.Duration;
import java.util.*;
import java.util.concurrent.TimeUnit;

import static com.automatak.dnp3.impl.mocks.ExpectedValue.Type.FrozenCounterType;

public class StackPair {

    public static final int LEVELS = LogLevels.INFO;

    final int NUM_POINTS_PER_TYPE;
    final int EVENTS_PER_ITERATION;

    static OutstationStackConfig getOutstationConfig(int numPointsPerType, int eventBufferSize)
    {
        OutstationStackConfig config = new OutstationStackConfig(
                DatabaseConfig.allValues(numPointsPerType), EventBufferConfig.allTypes(eventBufferSize)
        );

        config.outstationConfig.unsolConfirmTimeout = Duration.ofSeconds(1);

        config.outstationConfig.allowUnsolicited = true;

        return config;
    }

    static MasterStackConfig getMasterStackConfig()
    {
        MasterStackConfig config = new MasterStackConfig();

        config.master.responseTimeout = Duration.ofSeconds(1);
        config.master.taskRetryPeriod = Duration.ofSeconds(1);


        config.master.disableUnsolOnStartup = false;
        config.master.startupIntegrityClassMask = ClassField.none();
        config.master.unsolClassMask = ClassField.allEventClasses();

        return config;
    }

    public StackPair(DNP3Manager manager, int port, int numPointsPerType, int eventsPerIteration)
    {
        this.NUM_POINTS_PER_TYPE = numPointsPerType;
        this.EVENTS_PER_ITERATION = eventsPerIteration;

        try {

            Channel client = manager.addTCPClient(
                    String.format("client:%d", port),
                    LEVELS,
                    ChannelRetry.getDefault(),
                    Arrays.asList(new IPEndpoint("127.0.0.1", port)),
                    "127.0.0.1",
                    clientListener);

            Channel server = manager.addTCPServer(
                    String.format("server:%d", port),
                    LEVELS | LogLevels.APP_HEADER_TX | LogLevels.APP_OBJECT_TX,
                    ServerAcceptMode.CloseExisting,
                    new IPEndpoint("127.0.0.1", port),
                    serverListener);

            this.master = client.addMaster(
                    String.format("master:%d", port),
                    this.soeHandler,
                    DefaultMasterApplication.getInstance(),
                    getMasterStackConfig());

            this.outstation = server.addOutstation(
                    String.format("outstation:%d", port),
                    this.commandHandler,
                    DefaultOutstationApplication.getInstance(),
                    getOutstationConfig(numPointsPerType, 2*eventsPerIteration));
        }
        catch(DNP3Exception ex)
        {
            throw new RuntimeException(ex);
        }

        this.outstation.enable();
        this.master.enable();
    }

    public void waitForChannelsOpen(Duration duration)
    {
        this.clientListener.waitFor(ChannelState.OPEN, duration);
        this.serverListener.waitFor(ChannelState.OPEN, duration);
    }

    // transmit some random values via the outstation
    public int sendRandomValues()
    {
        final OutstationChangeSet set = new OutstationChangeSet();

        for(int i = 0; i < this.EVENTS_PER_ITERATION; ++i)
        {
           ExpectedValue value = this.addRandomValue(set);
           this.sentValues.add(value);
        }

        this.outstation.apply(set);

        return this.EVENTS_PER_ITERATION;
    }

    public void awaitSentValues(Duration duration)
    {
        final int total = sentValues.size();

        List<ExpectedValue> receivedValues = soeHandler.waitForValues(total, duration);

        if(receivedValues == null)
        {
            throw new RuntimeException("No values received within timeout");
        }

        if(receivedValues.size() != sentValues.size())
        {
            throw new RuntimeException(String.format("# sent (%d) != # received (%d)", total, receivedValues.size()));
        }

        int numValidated = 0;

        for(ExpectedValue received : receivedValues)
        {
            ExpectedValue expected = sentValues.poll();

            if(!expected.isEqual(received))
            {
                throw new RuntimeException(String.format("received %s != expected %s w/ num validated %d", received, expected, numValidated));
            }

            ++numValidated;
        }

    }

    public ExpectedValue addRandomValue(OutstationChangeSet set)
    {
        final int index = random.nextInt(NUM_POINTS_PER_TYPE);

        final ExpectedValue.Type type = getRandomElement(ExpectedValue.ALL_TYPES);

        // Note: It's important to use EventMode force here, because otherwise changes might not happen b/c of random value collisions

        switch(type)
        {
            case BinaryType: {
                BinaryInput v = new BinaryInput(random.nextBoolean(), new Flags((byte)0x01), new DNPTime(0));
                set.update(v, index, EventMode.Force);
                return new ExpectedValue(v, index);
            }
            case DoubleBinaryType: {
                DoubleBitBinaryInput v = new DoubleBitBinaryInput(getRandomElement(DoubleBit.values()), new Flags((byte)0x01), new DNPTime(0));
                set.update(v, index, EventMode.Force);
                return new ExpectedValue(v, index);
            }
            case CounterType: {
                Counter v = new Counter(random.nextInt(65535), new Flags((byte)0x01), new DNPTime(0));
                set.update(v, index, EventMode.Force);
                return new ExpectedValue(v, index);
            }
            case FrozenCounterType: {
                set.freezeCounter(index, false, EventMode.Force);
                return new ExpectedValue(index, FrozenCounterType);
            }
            case AnalogType: {
                AnalogInput v = new AnalogInput(random.nextInt(65535), new Flags((byte)0x01), new DNPTime(0));
                set.update(v, index, EventMode.Force);
                return new ExpectedValue(v, index);
            }
            case BOStatusType: {
                BinaryOutputStatus v = new BinaryOutputStatus(random.nextBoolean(), new Flags((byte)0x01), new DNPTime(0));
                set.update(v, index, EventMode.Force);
                return new ExpectedValue(v, index);
            }
            case AOStatusType: {
                AnalogOutputStatus v = new AnalogOutputStatus(random.nextInt(65535), new Flags((byte)0x01), new DNPTime(0));
                set.update(v, index, EventMode.Force);
                return new ExpectedValue(v, index);
            }
            default:
                throw new RuntimeException("unknown random type: " + type);
        }

    }

    public int sendRandomCommands() {
        for(int i = 0; i < this.EVENTS_PER_ITERATION; ++i)
        {
            ExpectedCommand command = this.addRandomCommand();
            this.sentCommands.add(command);
        }

        return this.EVENTS_PER_ITERATION;
    }

    public void awaitSentCommands(Duration duration)
    {
        final int total = sentCommands.size();

        List<ExpectedCommand> receivedCommands = commandHandler.waitForCommands(total, duration);

        if(receivedCommands == null)
        {
            throw new RuntimeException("No values received within timeout");
        }

        if(receivedCommands.size() != sentCommands.size())
        {
            throw new RuntimeException(String.format("# sent (%d) != # received (%d)", total, receivedCommands.size()));
        }

        int numValidated = 0;

        for(ExpectedCommand received : receivedCommands)
        {
            ExpectedCommand expected = sentCommands.poll();

            if(!expected.equals(received))
            {
                throw new RuntimeException(String.format("received %s != expected %s w/ num validated %d", received, expected, numValidated));
            }

            ++numValidated;
        }

    }

    public ExpectedCommand addRandomCommand()
    {
        final int index = random.nextInt(NUM_POINTS_PER_TYPE);

        final ExpectedCommand.Type type = getRandomElement(ExpectedCommand.ALL_TYPES);

        switch(type)
        {
            case Crob: {
                ControlRelayOutputBlock crob = new ControlRelayOutputBlock(
                    getRandomElement(new OperationType[] {OperationType.NUL, OperationType.PULSE_ON, OperationType.PULSE_OFF, OperationType.LATCH_ON, OperationType.LATCH_OFF}),
                    getRandomElement(new TripCloseCode[] {TripCloseCode.NUL, TripCloseCode.CLOSE, TripCloseCode.TRIP}),
                    random.nextBoolean(),
                    (short)random.nextInt(255),
                    Math.abs(random.nextLong()) % 4294967295L,
                    Math.abs(random.nextLong()) % 4294967295L,
                    getRandomElement(CommandStatus.values())
                );
                this.master.directOperateCROB(crob, index);
                return ExpectedCommand.build(crob, index);
            }
            case AoInt16: {
                AnalogOutputInt16 ao = new AnalogOutputInt16(
                        (short)random.nextInt(65535),
                        getRandomElement(CommandStatus.values())
                );
                this.master.directOperateAOInt16(ao, index);
                return ExpectedCommand.build(ao, index);
            }
            case AoInt32: {
                AnalogOutputInt32 ao = new AnalogOutputInt32(
                        (short)random.nextInt(),
                        getRandomElement(CommandStatus.values())
                );
                this.master.directOperateAOInt32(ao, index);
                return ExpectedCommand.build(ao, index);
            }
            case AoFloat32: {
                AnalogOutputFloat32 ao = new AnalogOutputFloat32(
                        (short)random.nextFloat(),
                        getRandomElement(CommandStatus.values())
                );
                this.master.directOperateAOFloat32(ao, index);
                return ExpectedCommand.build(ao, index);
            }
            case AoDouble64: {
                AnalogOutputDouble64 ao = new AnalogOutputDouble64(
                        (short)random.nextDouble(),
                        getRandomElement(CommandStatus.values())
                );
                this.master.directOperateAODouble64(ao, index);
                return ExpectedCommand.build(ao, index);
            }
            default:
                throw new RuntimeException("unknown random type: " + type);
        }

    }

    public <T> T getRandomElement(T[] items)
    {
        return items[random.nextInt(items.length)];
    }

    final BlockingChannelListener clientListener = new BlockingChannelListener();
    final BlockingChannelListener serverListener = new BlockingChannelListener();
    final QueuedSOEHandler soeHandler = new QueuedSOEHandler();
    final Queue<ExpectedValue> sentValues = new ArrayDeque<>();
    final QueuedCommandHandler commandHandler = new QueuedCommandHandler();
    final Queue<ExpectedCommand> sentCommands = new ArrayDeque<>();
    final Random random = new Random();

    final Master master;
    final Outstation outstation;

}
