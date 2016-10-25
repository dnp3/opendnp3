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
package com.automatak.dnp3;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

/**
 * Represents the 48-bit DNP3 time
 * Created by Sam on 7/25/2016.
 */
public class DNPTime {
    public DNPTime()
    {
        milliSecondSinceEpoch =0;
    }

    public DNPTime(long time)
    {
        assert(time>=MIN_VALUE && time <=MAX_VALUE);
        milliSecondSinceEpoch =time;
    }

    public long getMsSinceEpoch() {
        return milliSecondSinceEpoch;
    }

    public final long MAX_VALUE = 0xffffffffffffL;
    public final long MIN_VALUE = 0x0L;

    public long milliSecondSinceEpoch;

    @Override
    public String toString()
    {
        Date date = new Date(milliSecondSinceEpoch);
        DateFormat formatter = new SimpleDateFormat("YYYY/MM/dd HH:mm:ss.SSS ");
        formatter.setTimeZone(TimeZone.getTimeZone("UTC"));
        return formatter.format(date);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        DNPTime dnpTime = (DNPTime) o;

        return milliSecondSinceEpoch == dnpTime.milliSecondSinceEpoch;

    }

    @Override
    public int hashCode() {
        return (int) (milliSecondSinceEpoch ^ (milliSecondSinceEpoch >>> 32));
    }
}
