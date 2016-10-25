/**
 * Copyright 2013-2016 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3.impl.mocks;

import com.automatak.dnp3.*;

import java.time.Duration;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;

public class QueuedSOEHandler implements SOEHandler {

    final LinkedBlockingQueue<ExpectedValue> items = new LinkedBlockingQueue<>();

    static <T> Collection<ExpectedValue> map(Iterable<T> values, Function<T, ExpectedValue> convert)
    {
        ArrayList<ExpectedValue> items = new ArrayList<>();
        values.forEach(v -> items.add(convert.apply(v)));
        return items;
    }

    public void expect(Queue<ExpectedValue> expectedValues, Duration duration)
    {
        int numValidated = 0;
        final int total = expectedValues.size();

        try
        {
            while(!expectedValues.isEmpty())
            {
                ExpectedValue expected = expectedValues.poll();

                ExpectedValue received = items.poll(duration.toMillis(), TimeUnit.MILLISECONDS);

                if(received == null) {
                    throw new RuntimeException(String.format("Timeout waiting for value %s (%d/%d)", expected, numValidated+1, total));
                }

                if(!expected.isEqual(received))
                {
                    throw new RuntimeException(String.format("%s != %s w/ num validated %d", expected, received, numValidated));
                }

                ++numValidated;
            }

        }
        catch(InterruptedException ex)
        {
            throw new RuntimeException(ex);
        }
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

    @Override
    public void processTime(HeaderInfo info, Iterable<DNPTime> values)
    {
        items.addAll(map(values, v -> new ExpectedValue(v,0)));
    }
}
