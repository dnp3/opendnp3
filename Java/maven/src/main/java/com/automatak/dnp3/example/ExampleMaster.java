package com.automatak.dnp3.example;


import com.automatak.dnp3.*;
import com.automatak.dnp3.impl.ManagerImpl;

public class ExampleMaster {

    public static void main(String[] args) throws InterruptedException
    {
        //System.load("/home/adam/dev/dnp3/Java/cpp/Linux_i686/release/libopendnp3java.so");
        System.load("E:\\git\\dnp3\\Debug\\opendnp3java.dll");

        DNP3Manager mgr = new ManagerImpl(4);

        mgr.addLogSubscriber(new PrintingLogSubscriber());

        Channel server = mgr.addTCPServer("server", LogLevel.INFO, 5000, "127.0.0.1", 20000);
        Outstation outstation = server.addOutstation("outstation", LogLevel.INFO, new ConstantCommandHandler(CommandStatus.HARDWARE_ERROR));
        Channel client = mgr.addTCPClient("client", LogLevel.INFO, 5000, "127.0.0.1", 20000);
        Master master = client.addMaster("master", LogLevel.INFO, new PrintingDataObserver());

        Thread.sleep(10000);

        mgr.shutdown();

    }
}
