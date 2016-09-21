package com.automatak.dnp3;

/**
 * An interface that can have any command type collection added to it
 */
public interface CommandBuilder
{
    void addCrob(Iterable<IndexedValue<ControlRelayOutputBlock>> commands);
    void addAO16(Iterable<IndexedValue<AnalogOutputInt16>> commands);
    void addAO32(Iterable<IndexedValue<AnalogOutputInt32>> commands);
    void addAOFloat32(Iterable<IndexedValue<AnalogOutputFloat32>> commands);
    void addAODouble64(Iterable<IndexedValue<AnalogOutputDouble64>> commands);
};