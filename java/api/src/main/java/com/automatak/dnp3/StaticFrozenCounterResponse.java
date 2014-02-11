/**
 * Copyright 2013 Automatak, LLC
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

/**
 * Created by jstone on 2/11/14.
 */
public enum StaticFrozenCounterResponse {
    Group21Var1(0),
    Group21Var2(1),
    Group21Var5(2),
    Group21Var6(3),
    Group21Var9(4),
    Group21Var10(5);

    private final int id;

    private StaticFrozenCounterResponse(int id)
    {
        this.id = id;
    }

    public int toType()
    {
        return id;
    }

    public static StaticFrozenCounterResponse fromType(int arg)
    {
        switch(arg)
        {
            case(0):
                return Group21Var1;
            case(1):
                return Group21Var2;
            case(2):
                return Group21Var5;
            case(3):
                return Group21Var6;
            case(4):
                return Group21Var9;
            case(5):
                return Group21Var10;
        }

        return Group21Var1;
    }

}
