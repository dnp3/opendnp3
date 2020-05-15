/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
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


import com.automatak.dnp3.enums.StaticTypeBitmask;

/**
 * A bitfield that describes a subset of all static types, e.g. { Binary, Analog } or {Analog, Counter, FrozenCounter }
 */
public class StaticTypeBitField {

    public int bitfield;

    private StaticTypeBitField(int mask) {
        this.bitfield = mask;
    }

    public static StaticTypeBitField from(StaticTypeBitmask... types) {
        byte mask = 0;
        for (StaticTypeBitmask type : types) {
            mask |= type.toType();
        }
        return new StaticTypeBitField(mask);
    }

    public boolean isSet(StaticTypeBitmask type)
    {
        return (type.toType() & bitfield) != 0;
    }

    public void set(StaticTypeBitmask type, boolean value)
    {
        if(value)
        {
            this.bitfield |= type.toType();
        }
        else
        {
            this.bitfield &= ~type.toType();
        }
    }

    public static StaticTypeBitField none() {
        return new StaticTypeBitField(0);
    }

    public static StaticTypeBitField all() {
        return new StaticTypeBitField(~0);
    }

    public StaticTypeBitField except(StaticTypeBitmask type) {
        return new StaticTypeBitField(this.bitfield & ~type.toType());
    }

}
