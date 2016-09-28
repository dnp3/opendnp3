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

        for (Consumer action : updates) {
            action.accept(database);
        }
    }

    public void update(BinaryInput update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    public void update(DoubleBitBinaryInput update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    public void update(AnalogInput update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    public void update(Counter update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    public void update(FrozenCounter update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

    public void update(BinaryOutputStatus update, int index) {
        updates.add((Database db) -> db.update(update, index, EventMode.Detect));
    }

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
    public void update(FrozenCounter update, int index, EventMode mode) {
        updates.add((Database db) -> db.update(update, index, mode));
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
