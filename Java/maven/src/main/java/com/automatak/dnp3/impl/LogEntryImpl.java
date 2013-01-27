package com.automatak.dnp3.impl;

import com.automatak.dnp3.LogEntry;
import com.automatak.dnp3.LogLevel;

import java.util.Date;

class LogEntryImpl implements LogEntry {

    private final int filterLevel;
    private final String loggerName;
    private final String message;
    private final Date date;
    private final int errorCode;

    public LogEntryImpl(int filterLevel, String loggerName, String message, long timeMsSinceEpoch, int errorCode)
    {
        this.filterLevel = filterLevel;
        this.loggerName = loggerName;
        this.message = message;
        this.date = new Date(timeMsSinceEpoch);
        this.errorCode = errorCode;
    }

    public LogLevel getLogLevel()
    {
        switch(filterLevel)
        {
            case(0x01):
                return LogLevel.EVENT;
            case(0x02):
                return LogLevel.ERROR;
            case(0x04):
                return LogLevel.WARNING;
            case(0x08):
                return LogLevel.INFO;
            case(0x10):
                return LogLevel.INTERPRET;
            case(0x20):
                return LogLevel.COMM;
            case(0x40):
                return LogLevel.DEBUG;
            default:
                throw new IllegalArgumentException("Invalid native log level: " + filterLevel);
        }
    }

    public String getLoggerName()
    {
        return loggerName;
    }

    public String getMessage()
    {
        return message;
    }

    public Date getTimestamp()
    {
        return date;
    }

    public int getErrorCode()
    {
        return errorCode;
    }

}
