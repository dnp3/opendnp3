/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3.impl;

import com.automatak.dnp3.LogEntry;
import com.automatak.dnp3.LogLevel;

class LogEntryImpl implements LogEntry {

    private final LogLevel filterLevel;
    private final String loggerName;
    private final String message;
    private final int errorCode;

    public LogEntryImpl(int filterLevel, String loggerName, String message, int errorCode)
    {
        this.filterLevel = convertLogLevel(filterLevel);
        this.loggerName = loggerName;
        this.message = message;
        this.errorCode = errorCode;
    }

    public static LogLevel convertLogLevel(int level)
    {
        switch(level)
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
                return LogLevel.ERROR;
        }
    }

    public LogLevel getLogLevel()
    {
        return filterLevel;
    }

    public String getLoggerName()
    {
        return loggerName;
    }

    public String getMessage()
    {
        return message;
    }

    public int getErrorCode()
    {
        return errorCode;
    }

    @Override
    public String toString()
    {
        StringBuilder sb = new StringBuilder();
        sb.append(filterLevel);
        sb.append(" - ");
        sb.append(loggerName);
        sb.append(" - ");
        sb.append(message);
        return sb.toString();
    }

}
