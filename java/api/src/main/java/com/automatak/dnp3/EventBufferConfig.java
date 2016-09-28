package com.automatak.dnp3;

/**
 * Configuration of maximum event counts per event type.
 * <p>
 * The underlying implementation uses a *preallocated* heap buffer to store events
 * until they are transmitted to the master. The size of this buffer is proportional
 * to the TotalEvents() method, i.e. the sum of the maximum events for each type.
 * <p>
 * Implementations should configure the buffers to store a reasonable number events
 * given the polling frequency and memory restrictions of the target platform.
 */
public class EventBufferConfig {

    /**
     * Construct the class using the same maximum for all types. This is mainly used for demo purposes.
     * You probably don't want to use this method unless your implementation actually reports every type.
     *
     * @param sizes size parameter for every type
     * @return a new EventBufferConfig instance
     */
    public static EventBufferConfig allTypes(int sizes) {
        return new EventBufferConfig(0, 0, 0, 0, 0, 0, 0);
    }

    /**
     * Construct the class specifying the maximum number of events for each type individually.
     */
    public EventBufferConfig(
            int maxBinaryEvents,
            int maxDoubleBinaryEvents,
            int maxAnalogEvents,
            int maxCounterEvents,
            int maxFrozenCounterEvents,
            int maxBinaryOutputStatusEvents,
            int maxAnalogOutputStatusEvents)
    {
        this.maxBinaryEvents = maxBinaryEvents;
        this.maxDoubleBinaryEvents = maxDoubleBinaryEvents;
        this.maxAnalogEvents = maxAnalogEvents;
        this.maxCounterEvents = maxCounterEvents;
        this.maxFrozenCounterEvents = maxFrozenCounterEvents;
        this.maxBinaryOutputStatusEvents = maxBinaryOutputStatusEvents;
        this.maxAnalogOutputStatusEvents = maxAnalogOutputStatusEvents;
    }

    /// The number of binary events the outstation will buffer before overflowing
    int maxBinaryEvents;

    /// The number of double bit binary events the outstation will buffer before overflowing
    int maxDoubleBinaryEvents;

    /// The number of analog events the outstation will buffer before overflowing
    int maxAnalogEvents;

    /// The number of counter events the outstation will buffer before overflowing
    int maxCounterEvents;

    /// The number of frozen counter events the outstation will buffer before overflowing
    int maxFrozenCounterEvents;

    /// The number of binary output status events the outstation will buffer before overflowing
    int maxBinaryOutputStatusEvents;

    /// The number of analog output status events the outstation will buffer before overflowing
    int maxAnalogOutputStatusEvents;

};


