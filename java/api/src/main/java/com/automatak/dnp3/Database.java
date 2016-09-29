package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventMode;

/**
 * Interface used to load measurement changes into an outstation
 */
public interface Database
{
    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     */
    void update(BinaryInput value, int index);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     */
    void update(DoubleBitBinaryInput value, int index);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     */
    void update(AnalogInput value, int index);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     */
    void update(Counter value, int index);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     */
    void update(FrozenCounter value, int index);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     */
    void update(BinaryOutputStatus value, int index);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     */
    void update(AnalogOutputStatus value, int index);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     * @param mode EventMode to use
     */
    void update(BinaryInput value, int index, EventMode mode);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     * @param mode EventMode to use
     */
    void update(DoubleBitBinaryInput value, int index, EventMode mode);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     * @param mode EventMode to use
     */
    void update(AnalogInput value, int index, EventMode mode);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     * @param mode EventMode to use
     */
    void update(Counter value, int index, EventMode mode);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     * @param mode EventMode to use
     */
    void update(FrozenCounter value, int index, EventMode mode);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     * @param mode EventMode to use
     */
    void update(BinaryOutputStatus value, int index, EventMode mode);

    /**
     * Update a value in the database
     * @param value measurement to update
     * @param index index of measurement
     * @param mode EventMode to use
     */
    void update(AnalogOutputStatus value, int index, EventMode mode);

}
