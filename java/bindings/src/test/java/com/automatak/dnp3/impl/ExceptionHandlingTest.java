package com.automatak.dnp3.impl;

import com.automatak.dnp3.*;
import com.automatak.dnp3.impl.mocks.NullLogHandler;
import com.automatak.dnp3.mock.PrintingChannelListener;
import org.junit.Test;

import java.util.Collections;

public class ExceptionHandlingTest {

    @Test(expected = DNP3Exception.class)
    public void testCreateTcpClientFromShutdownManager() throws DNP3Exception {
        DNP3Manager manager = DNP3ManagerFactory.createManager(4, new NullLogHandler());
        manager.shutdown();

        Channel channel = manager.addTCPClient(
                "client",
                LogMasks.NORMAL | LogMasks.APP_COMMS,
                ChannelRetry.getDefault(),
                Collections.singletonList(new IPEndpoint("127.0.0.1", 20000)),
                "0.0.0.0",
                PrintingChannelListener.getInstance()
        );
    }
}
