package com.automatak.dnp3;


/**
 * Configuration object for the application layer
 */
public class AppLayerConfig {

    /**
     * Response/confirm timeout in milliseconds
     */
    public long rspTimeoutMs;

    /**
     *  Number of retries performed for allowed frames. Frame like Select/Operate/etc are not retried
     */
    public int numRetry;

    /**
     * Maximum parsable fragment size of received APDU. Typically the default of 2048 is OK.
     */
    public int maxFragSize;


    /**
     * Constructor with reasonable defaults
     */
    public AppLayerConfig()
    {
      this.rspTimeoutMs = 5000;
      this.numRetry = 0;
      this.maxFragSize = 2048;
    }

}
