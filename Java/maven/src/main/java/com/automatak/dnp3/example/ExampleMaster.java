package com.automatak.dnp3.example;


import com.automatak.dnp3.*;
import com.automatak.dnp3.impl.ManagerImpl;



public class ExampleMaster {

    public static void main(String[] args) throws InterruptedException
    {
        System.load("/home/adam/dev/dnp3/Java/cpp/Linux_i686/release/libopendnp3java.so");

        DNP3Manager mgr = new ManagerImpl(4);

        mgr.addLogSubscriber(new PrintingLogSubscriber());

        Channel server = mgr.addTCPServer("server", LogLevel.INFO, 500, "127.0.0.1", 20000);
        Outstation outstation = server.addOutstation("outstation", LogLevel.INFO, null);
        Channel client = mgr.addTCPClient("client", LogLevel.INFO, 500, "127.0.0.1", 20000);
        Master master = client.addMaster("master", LogLevel.INFO, new PrintingDataObserver());


        Thread.sleep(2000);

        mgr.shutdown();

    }
}
