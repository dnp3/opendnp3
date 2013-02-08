package com.automatak.dnp3.example;

import com.automatak.dnp3.*;
import com.automatak.dnp3.impl.DNP3ManagerFactory;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Example master than can be run against the example outstation
 */
public class OutstationDemo {

    public static void main(String[] args) throws IOException, InterruptedException {

        // create the root class with a thread pool size of 1
        DNP3Manager manager = DNP3ManagerFactory.createManager(1);

        // You can send the log messages anywhere you want
        // but PrinintLogSubscriber just prints them to the console
        manager.addLogSubscriber(new PrintingLogSubscriber());

        // Create a tcp channel class that will connect to the loopback
        Channel channel = manager.addTCPServer("client", LogLevel.INFO, 5000, "127.0.0.1", 20000);

        // Outstation will have 5 of every measurement type
        DatabaseConfig db = new DatabaseConfig(5,5,5,5,5);

        // Create the default outstation configuration
        OutstationStackConfig config = new OutstationStackConfig(db);

        // Create an Outstation instance, pass in a simple a command handler that responds successfully to everyhing
        Outstation outstation = channel.addOutstation("outstation", LogLevel.INTERPRET, SuccessCommandHandler.getInstance(), config);

        // This sub-interface allows us to load data into the outstation
        DataObserver data = outstation.getDataObserver();

        // all this stuff just to read a line of text in Java. Oh the humanity.
        String line = "";
        System.out.println("Enter a line of text (type 'quit' to exit): ");
        InputStreamReader converter = new InputStreamReader(System.in);
        BufferedReader in = new BufferedReader(converter);


        int i = 0;
        while (true) {
            System.out.println("Enter something to update a counter or type <quit> to exit");
            line = in.readLine();
            if(line.equals("quit")) break;
            else {
                data.start();
                data.update(new Counter(i, CounterInputQuality.ONLINE.toByte(), 0), 0);
                data.end();
                ++i;
            }
        }

        manager.shutdown();
    }
}
