package com.automatak.dnp3.example;

import com.automatak.dnp3.LogEntry;
import com.automatak.dnp3.LogSubscriber;

public class PrintingLogSubscriber implements LogSubscriber {

    public void onLogEntry(LogEntry entry)
    {
        System.out.println(Thread.currentThread().getId() + " : " + entry.getTimestamp() + " : " + entry.getLoggerName() + " - " + entry.getMessage());
    }
}