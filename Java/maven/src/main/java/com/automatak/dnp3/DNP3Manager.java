package com.automatak.dnp3;

public interface DNP3Manager {

    Channel addTCPClient(String name, LogLevel level, long retryMs, String address, int port);

    void shutdown();
}
