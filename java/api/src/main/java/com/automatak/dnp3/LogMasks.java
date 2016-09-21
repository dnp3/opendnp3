package com.automatak.dnp3;

public final class LogMasks
{
    public static final int NONE = 0;
    public static final int ALL = ~NONE;
    public static final int NORMAL = LogLevels.EVENT | LogLevels.ERROR | LogLevels.WARNING | LogLevels.INFO;
    public static final int APP_COMMS = LogLevels.APP_HEADER_RX | LogLevels.APP_HEADER_TX | LogLevels.APP_OBJECT_RX | LogLevels.APP_OBJECT_TX;
}
