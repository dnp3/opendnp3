/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3.impl;

import com.automatak.dnp3.*;

class DataObserverAdapter {
    private final DataObserver proxy;

    public DataObserverAdapter(DataObserver proxy)
    {
        this.proxy = proxy;
    }

    public void start()
    {
        proxy.start();
    }

    public void updateBI(boolean value, byte quality, long time, long index)
    {
        proxy.update(new BinaryInput(value, quality, time), index);
    }

    public void updateAI(double value, byte quality, long time, long index)
    {
        proxy.update(new AnalogInput(value, quality, time), index);
    }

    public void updateC(long value, byte quality, long time, long index)
    {
        proxy.update(new Counter(value, quality, time), index);
    }

    public void updateBOS(boolean value, byte quality, long time, long index)
    {
        proxy.update(new BinaryOutputStatus(value, quality, time), index);
    }

    public void updateAOS(double value, byte quality, long time, long index)
    {
        proxy.update(new AnalogOutputStatus(value, quality, time), index);
    }

    public void end()
    {
        proxy.end();
    }
}
