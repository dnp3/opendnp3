package com.automatak.dnp3.impl.mocks;

import com.automatak.dnp3.*;

import java.util.ArrayList;
import java.util.Collection;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.function.Function;

public class QueuedSOEHandler implements SOEHandler {

    final BlockingQueue<ExpectedValue> items = new LinkedBlockingQueue<>();

    static <T> Collection<ExpectedValue> map(Iterable<T> values, Function<T, ExpectedValue> convert)
    {
        ArrayList<ExpectedValue> items = new ArrayList<>();
        values.forEach(v -> items.add(convert.apply(v)));
        return items;
    }

    @Override
    public void start() {}

    @Override
    public void end() {}

    @Override
    public void processBI(HeaderInfo info, Iterable<IndexedValue<BinaryInput>> values)
    {
        items.addAll(map(values, v -> new ExpectedValue(v.value, v.index)));
    }

    @Override
    public void processDBI(HeaderInfo info, Iterable<IndexedValue<DoubleBitBinaryInput>> values)
    {
        items.addAll(map(values, v -> new ExpectedValue(v.value, v.index)));
    }


    @Override
    public void processAI(HeaderInfo info, Iterable<IndexedValue<AnalogInput>> values)
    {
        items.addAll(map(values, v -> new ExpectedValue(v.value, v.index)));
    }


    @Override
    public void processC(HeaderInfo info, Iterable<IndexedValue<Counter>> values)
    {
        items.addAll(map(values, v -> new ExpectedValue(v.value, v.index)));
    }


    @Override
    public void processFC(HeaderInfo info, Iterable<IndexedValue<FrozenCounter>> values)
    {
        items.addAll(map(values, v -> new ExpectedValue(v.value, v.index)));
    }


    @Override
    public void processBOS(HeaderInfo info, Iterable<IndexedValue<BinaryOutputStatus>> values)
    {
        items.addAll(map(values, v -> new ExpectedValue(v.value, v.index)));
    }


    @Override
    public void processAOS(HeaderInfo info, Iterable<IndexedValue<AnalogOutputStatus>> values)
    {
        items.addAll(map(values, v -> new ExpectedValue(v.value, v.index)));
    }
}
