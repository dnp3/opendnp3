package com.automatak.dnp3.example;


import com.automatak.dnp3.Channel;
import com.automatak.dnp3.DNP3Manager;
import com.automatak.dnp3.LogLevel;
import com.automatak.dnp3.Master;
import com.automatak.dnp3.impl.ManagerImpl;

public class ExampleMaster {

    public static void main(String[] args) throws InterruptedException
    {
        System.load("/home/adam/dev/dnp3/Java/cpp/Linux_i686/release/libopendnp3java.so");

        DNP3Manager mgr = new ManagerImpl(4);

        Channel client = mgr.addTCPClient("client", LogLevel.INFO, 3000, "127.0.0.1", 20000);
        Master master = client.addMaster("master", LogLevel.INFO);

        Thread.sleep(5000);

        mgr.shutdown();
    }
}
