package com.automatak.dnp3;

import java.util.function.Consumer;

/// <summary>
/// Concrete implementation of ICommandHeader
/// </summary>
public class ActionCommandHeader implements CommandHeaders {

    private final Consumer<CommandBuilder> action;

    @Override
    public void build(CommandBuilder builder) {
        action.accept(builder);
    }

    private ActionCommandHeader(Consumer<CommandBuilder> action) {
        this.action = action;
    }

    public static CommandHeaders fromCROB(Iterable<IndexedValue<ControlRelayOutputBlock>> commands) {
        return new ActionCommandHeader((CommandBuilder builder) -> builder.addCrob(commands));
    }

    public static CommandHeaders fromAO16(Iterable<IndexedValue<AnalogOutputInt16>> commands) {
        return new ActionCommandHeader((CommandBuilder builder) -> builder.addAO16(commands));
    }

    public static CommandHeaders fromAO32(Iterable<IndexedValue<AnalogOutputInt32>> commands) {
        return new ActionCommandHeader((CommandBuilder builder) -> builder.addAO32(commands));
    }

    public static CommandHeaders fromAOFloat(Iterable<IndexedValue<AnalogOutputFloat32>> commands) {
        return new ActionCommandHeader((CommandBuilder builder) -> builder.addAOFloat32(commands));
    }

    public static CommandHeaders fromAODouble(Iterable<IndexedValue<AnalogOutputDouble64>> commands) {
        return new ActionCommandHeader((CommandBuilder builder) -> builder.addAODouble64(commands));
    }

}
