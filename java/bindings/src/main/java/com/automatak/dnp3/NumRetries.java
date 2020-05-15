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

/**
 * Number of unsolicited response retries
 */
public class NumRetries {

    /**
     * Create a fixed number of maximum unsolicited response retries
     * @param maxNumRetries Maximum number of retries
     * @return Fixed number of retries
     */
    static public NumRetries Fixed(int maxNumRetries)
    {
        return new NumRetries(maxNumRetries, false);
    }

    /**
     * Create an infinite number of unsolicited response retries
     * @return Infinite number of retries
     */
    static public NumRetries Infinite()
    {
        return new NumRetries(0, true);
    }

    private NumRetries(int maxNumRetries, boolean isInfinite)
    {
        this.maxNumRetries = maxNumRetries;
        this.isInfinite = isInfinite;
    }

    public final int maxNumRetries;
    public final boolean isInfinite;
}
