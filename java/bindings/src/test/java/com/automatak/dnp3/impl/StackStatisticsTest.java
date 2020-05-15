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
package com.automatak.dnp3.impl;

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

import com.automatak.dnp3.*;
import com.automatak.dnp3.impl.mocks.NullLogHandler;
import com.automatak.dnp3.mock.*;
import junit.framework.TestCase;
import org.junit.Assert;
import org.junit.Test;

import java.util.Arrays;

public class StackStatisticsTest {

    @Test
    public void testGetMasterStatistics() throws DNP3Exception {
        DNP3Manager manager = DNP3ManagerFactory.createManager(4, new NullLogHandler());

        Channel channel = manager.addTCPClient(
                "client",
                LogMasks.NORMAL | LogMasks.APP_COMMS,
                ChannelRetry.getDefault(),
                Arrays.asList(new IPEndpoint("127.0.0.1", 20000)),
                "0.0.0.0",
                PrintingChannelListener.getInstance()
        );
        Master master = channel.addMaster("master",
                PrintingSOEHandler.getInstance(),
                DefaultMasterApplication.getInstance(),
                new MasterStackConfig()
        );

        // This used to return nullptr, see issue #268
        Assert.assertNotNull(master.getStatistics().link);
        Assert.assertNotNull(master.getStatistics().transport);
    }

    @Test
    public void testGetOutstationStatistics() throws DNP3Exception {
        DNP3Manager manager = DNP3ManagerFactory.createManager(4, new NullLogHandler());

        Channel channel = manager.addTCPClient(
                "client",
                LogMasks.NORMAL | LogMasks.APP_COMMS,
                ChannelRetry.getDefault(),
                Arrays.asList(new IPEndpoint("127.0.0.1", 20000)),
                "0.0.0.0",
                PrintingChannelListener.getInstance()
        );
        Outstation outstation = channel.addOutstation("outstation",
                SuccessCommandHandler.getInstance(),
                DefaultOutstationApplication.getInstance(),
                new OutstationStackConfig(DatabaseConfig.allValues(5), EventBufferConfig.allTypes(5))
        );

        // This used to return nullptr, see issue #268
        Assert.assertNotNull(outstation.getStatistics().link);
        Assert.assertNotNull(outstation.getStatistics().transport);
    }

    @Test
    public void testGetChannelStatistics() throws DNP3Exception {
        DNP3Manager manager = DNP3ManagerFactory.createManager(4, new NullLogHandler());

        Channel channel = manager.addTCPClient(
                "client",
                LogMasks.NORMAL | LogMasks.APP_COMMS,
                ChannelRetry.getDefault(),
                Arrays.asList(new IPEndpoint("127.0.0.1", 20000)),
                "0.0.0.0",
                PrintingChannelListener.getInstance()
        );

        // This used to return nullptr, see issue #268
        Assert.assertNotNull(channel.getStatistics().channel);
        Assert.assertNotNull(channel.getStatistics().parser);
    }
}

