package com.automatak.dnp3;

/**
 * A measurement value and index pair
 * @param <T> The type of the measurement value
 */
public class IndexedValue<T>
{
    public final T value;
    public final int index;

    public IndexedValue(T value, int index)
    {
        this.value = value;
        this.index = index;
    }

    @Override
    public String toString() {
        return String.format("%d - %s", index, value);
    }
}