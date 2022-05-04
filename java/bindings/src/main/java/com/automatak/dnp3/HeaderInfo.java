/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.dnp3;

import com.automatak.dnp3.enums.GroupVariation;
import com.automatak.dnp3.enums.QualifierCode;
import com.automatak.dnp3.enums.TimestampQuality;

public class HeaderInfo
{
    public HeaderInfo(GroupVariation variation, QualifierCode qualifier, TimestampQuality tsquality, boolean isEvent, boolean flagsValid, int headerIndex)
    {
        this.variation = variation;
        this.qualifier = qualifier;
        this.tsquality = tsquality;
        this.isEvent = isEvent;
        this.flagsValid = flagsValid;
        this.headerIndex = headerIndex;
    }

    @Override
    public String toString() {
        return String.format("HeaderInfo(%s, %s, timestamp: %s, isEvent: %b, flagsValid: %b, #: %d)", variation, qualifier, tsquality, isEvent, flagsValid, headerIndex);
    }

    /**
     * An enum representing the group and variation for this header
     */
    public final GroupVariation variation;

    /**
     * The qualifier code used for this header
     */
    public final QualifierCode qualifier;

    /**
     * An enumeration describing the validity of the timestamp on the measurements
     */
    public final TimestampQuality tsquality;

    /**
     * True if this qualifier is an event type
     */
    public final boolean isEvent;

    /**
     * True if the flags/quality on the measurements are valid
     */
    public final boolean flagsValid;

    /**
     * The index of the header (0-based) within the ASDU
     */
    public final int headerIndex;
}
