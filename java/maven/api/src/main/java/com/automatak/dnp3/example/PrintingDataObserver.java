package com.automatak.dnp3.example;

/**
 * Simple singleton that prints measurements to the console as they are received
 */
public class PrintingDataObserver extends FormattingDataObserver {
    private static PrintingDataObserver ourInstance = new PrintingDataObserver();

    public static PrintingDataObserver getInstance() {
        return ourInstance;
    }

    private PrintingDataObserver() {
        super(new OutputHandler() {
            @Override
            public void handleOutput(String output) {
                System.out.println(output);
            }
        });
    }
}
