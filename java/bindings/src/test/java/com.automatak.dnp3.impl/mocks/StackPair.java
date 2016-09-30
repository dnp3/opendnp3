package com.automatak.dnp3.impl.mocks;


import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.ChannelState;
import com.automatak.dnp3.mock.DefaultMasterApplication;
import com.automatak.dnp3.mock.DefaultOutstationApplication;
import com.automatak.dnp3.mock.PrintingSOEHandler;
import com.automatak.dnp3.mock.SuccessCommandHandler;

import java.time.Duration;

public class StackPair {

    public static final int LEVELS = LogLevels.INFO;
    public static final int NUM_POINTS_PER_TYPE = 50;
    public static final int EVENT_BUFFER_SIZE = 50;

    static OutstationStackConfig getOutstationConfig()
    {
        OutstationStackConfig config = new OutstationStackConfig(
                DatabaseConfig.allValues(NUM_POINTS_PER_TYPE), EventBufferConfig.allTypes(EVENT_BUFFER_SIZE)
        );

        return config;
    }

    static MasterStackConfig getMasterStackConfig()
    {
        MasterStackConfig config = new MasterStackConfig();

        config.master.disableUnsolOnStartup = false;
        config.master.startupIntegrityClassMask = ClassField.none();
        config.master.unsolClassMask = ClassField.none();

        return config;
    }

    public StackPair(DNP3Manager manager, int port)
    {
        try {

            Channel client = manager.addTCPClient(
                    String.format("client:%d", port),
                    LEVELS, ChannelRetry.getDefault(),
                    "127.0.0.1",
                    "127.0.0.1",
                    port,
                    clientListener);

            Channel server = manager.addTCPServer(
                    String.format("server:%d", port),
                    LEVELS, ChannelRetry.getDefault(),
                    "127.0.0.1",
                    port,
                    serverListener);

            this.master = client.addMaster(
                    String.format("master:%d", port),
                    this.soeHandler,
                    DefaultMasterApplication.getInstance(),
                    getMasterStackConfig());

            this.outstation = server.addOutstation(
                    String.format("outstation:%d", port),
                    SuccessCommandHandler.getInstance(),
                    DefaultOutstationApplication.getInstance(),
                    getOutstationConfig());
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

    final BlockingChannelListener clientListener = new BlockingChannelListener();
    final BlockingChannelListener serverListener = new BlockingChannelListener();
    final QueuedSOEHandler soeHandler = new QueuedSOEHandler();

    final Master master;
    final Outstation outstation;

}
