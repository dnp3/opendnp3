package com.automatak.dnp3;

import java.util.Date;

public interface LogEntry {

    LogLevel getLogLevel();
    String getLoggerName();
    String getMessage();
    Date getTimestamp();
    int getErrorCode();

}
