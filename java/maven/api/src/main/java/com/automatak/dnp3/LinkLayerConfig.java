package com.automatak.dnp3;

/**
 * Configuration class for the link layer
 */
public class LinkLayerConfig {

    /**
     * Controls the master/slave bit set on all messages
     */
    boolean isMaster;

    /**
     * If true, the link layer will send data requesting confirmation. This is generally NEVER
     * set for a TCP connection and only sometimes set for serial.
     *
     * defaults to false
     */
    boolean useConfirms;

    /**
     * The number of retry attempts the link will attempt after the initial try if using confirms
     *
     * defaults to 0
     */
    int numRetry;

    /**
     * dnp3 address of the local device as a 16-bit unsigned integer
     */
    int localAddr;

    /**
     * dnp3 address of the remote device as a 16-bit unsigned integer
     */
    long remoteAddr;

    /**
     * the response timeout in milliseconds for confirmed requests
     */
    long timeoutMs;

    /**
     * Constructor that sets reasonable defaults. Override after construction
     * @param isMaster True if the link layer is for a master, false for an outstation
     */
    public LinkLayerConfig(boolean isMaster)
    {
        this.isMaster = isMaster;
        this.useConfirms = false;
        this .numRetry = 0;
        this.localAddr = isMaster ? 1 : 1024;
        this.remoteAddr = isMaster ? 1024: 1;
        this.timeoutMs = 1000;
    }

}
