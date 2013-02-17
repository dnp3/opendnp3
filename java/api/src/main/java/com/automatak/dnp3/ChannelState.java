package com.automatak.dnp3;

/**
 * Enumeration for possible states of a Channel
 */
public enum ChannelState {
    /**
     * offline and idle
     */
    CLOSED,
    /**
     * trying to open
     */
    OPENING,
    /**
     * waiting to open
     */
    WAITING,
    /**
     * open
     */
    OPEN,
    /**
     * stopped and will never do anything again
     */
    SHUTDOWN
}
