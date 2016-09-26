package com.automatak.dnp3;

public class Range {

    private static final int MIN_INDEX = 0;
    private static final int MAX_INDEX = 65535;

    public static Range from(int start, int stop) {
        return new Range(start, stop);
    }

    public static Range all() {
        return new Range(MIN_INDEX, MAX_INDEX);
    }

    public boolean isDefined()
    {
        return (start >= MIN_INDEX) && (stop <= MAX_INDEX) && (start <= stop);
    }

    public boolean isAllObjects() {
        return start == MIN_INDEX && stop == MAX_INDEX;
    }

    Range(int start, int stop) {
        this.start = start;
        this.stop = stop;
    }

    public final int start;
    public final int stop;
}
