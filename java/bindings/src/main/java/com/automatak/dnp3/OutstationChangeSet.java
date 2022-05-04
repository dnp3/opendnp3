/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements. 
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

import com.automatak.dnp3.enums.EventMode;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

/// <summary>
/// Concrete implementation of IChangeSet
/// </summary>
public class OutstationChangeSet implements Database, ChangeSet {

    private final List<Consumer<Database>> updates = new ArrayList<>();

    @Override
    public void apply(Database database) {

        for (Consumer<Database> action : updates) {
            action.accept(database);
        }
    }

    @Override
    public void update(BinaryInput update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    @Override
    public void update(DoubleBitBinaryInput update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    @Override
    public void update(AnalogInput update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    @Override
    public void update(Counter update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    @Override
    public void freezeCounter(int index, boolean clear) {
        updates.add((Database db) -> db.freezeCounter(index, clear, EventMode.Detect));
    }

    @Override
    public void update(BinaryOutputStatus update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    @Override
    public void update(AnalogOutputStatus update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    @Override
    public void update(BinaryInput update, int index, EventMode mode) {
        updates.add((Database db) -> db.update(update, index, mode));
    }

    @Override
    public void update(DoubleBitBinaryInput update, int index, EventMode mode) {
        updates.add((Database db) -> db.update(update, index, mode));
    }

    @Override
    public void update(AnalogInput update, int index, EventMode mode) {
        updates.add((Database db) -> db.update(update, index, mode));
    }

    @Override
    public void update(Counter update, int index, EventMode mode) {
        updates.add((Database db) -> db.update(update, index, mode));
    }

    @Override
    public void freezeCounter(int index, boolean clear, EventMode mode) {
        updates.add((Database db) -> db.freezeCounter(index, clear, mode));
    }

    @Override
    public void update(BinaryOutputStatus update, int index, EventMode mode) {
        updates.add((Database db) -> db.update(update, index, mode));
    }

    @Override
    public void update(AnalogOutputStatus update, int index, EventMode mode) {
        updates.add((Database db) -> db.update(update, index, mode));
    }


}
