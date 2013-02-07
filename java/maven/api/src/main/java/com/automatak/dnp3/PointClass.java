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
package com.automatak.dnp3;


public enum PointClass {
    CLASS_0(0x01),
    CLASS_1(0x02),
    CLASS_2(0x04),
    CLASS_3(0x08),
    ALL_EVENTS(CLASS_1.toInt() | CLASS_2.toInt() | CLASS_3.toInt());

    private final int id;

    PointClass(int id)
    {
        this.id = id;
    }

    int toInt()
    {
        return id;
    }

    public static int getMask(PointClass... args)
    {
        int i = 0;
        for(PointClass pc : args) { i |= pc.toInt(); }
        return i;
    }
}