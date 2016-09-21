package com.automatak.dnp3;

import java.time.Duration;


/**
 * Constrains the time range used for exponential backoff from some minimum to a maximum
 */
public class ChannelRetry
{
    /**
     * Construct a channel retry class
     * @param minRetryDelay minimum retry delay
     * @param maxRetryDelay maximum retry delay
     */
    public ChannelRetry(Duration minRetryDelay, Duration maxRetryDelay)
    {
        this.minRetryDelay = minRetryDelay;
        this.maxRetryDelay = maxRetryDelay;
    }

    /// <summary>
    /// Default configuration from 1 second to 1 minute
    /// </summary>
    public static ChannelRetry getDefault()
    {
        return new ChannelRetry(Duration.ofSeconds(1), Duration.ofMinutes(1));
    }

    public final Duration minRetryDelay;
    public final Duration maxRetryDelay;
}
