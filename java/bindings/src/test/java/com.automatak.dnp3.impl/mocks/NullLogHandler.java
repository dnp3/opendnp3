package com.automatak.dnp3.impl.mocks;

import com.automatak.dnp3.LogEntry;
import com.automatak.dnp3.LogHandler;

public class NullLogHandler implements LogHandler {

    @Override
    public void log(LogEntry entry) {}

}
