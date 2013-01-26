package com.automatak.dnp3.impl;

import com.automatak.dnp3.Channel;
import com.automatak.dnp3.DNP3Manager;
import com.automatak.dnp3.LogLevel;

class DNP3ManagerImpl implements DNP3Manager {

    public native Channel addTCPClient(String name, LogLevel level, long retryMs, String address, int port);

    public native void shutdown();

}
