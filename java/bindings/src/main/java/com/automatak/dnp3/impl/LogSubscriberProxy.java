package com.automatak.dnp3.impl;


import com.automatak.dnp3.LogSubscriber;

public final class LogSubscriberProxy {

    private LogSubscriber subscriber;

    public LogSubscriberProxy(LogSubscriber subscriber)
    {
        this.subscriber = subscriber;
    }

    public void onLogEntry(int filter, String name, String message, int errorCode)
    {
        LogEntryImpl entry = new LogEntryImpl(filter, name, message, errorCode);
        subscriber.onLogEntry(entry);
    }

}
