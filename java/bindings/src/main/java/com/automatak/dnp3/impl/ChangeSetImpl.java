package com.automatak.dnp3.impl;


import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.EventMode;

public class ChangeSetImpl implements Database {

    public final long nativePointer;

    public ChangeSetImpl()
    {
        this.nativePointer = create_instance_native();
    }

    @Override
    public void finalize()
    {
        this.destroy_instance_native(this.nativePointer);
    }

    @Override
    public void update(BinaryInput value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void update(DoubleBitBinaryInput value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void update(AnalogInput value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void update(Counter value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void update(FrozenCounter value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void update(BinaryOutputStatus value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void update(AnalogOutputStatus value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }


    @Override
    public void update(BinaryInput value, int index, EventMode mode)
    {
        this.update_binary_native(this.nativePointer, value.value, value.quality, value.timestamp, index, mode.toType());
    }

    @Override
    public void update(DoubleBitBinaryInput value, int index, EventMode mode)
    {
        this.update_double_binary_native(this.nativePointer, value.value.toType(), value.quality, value.timestamp, index, mode.toType());
    }

    @Override
    public void update(AnalogInput value, int index, EventMode mode)
    {
        this.update_analog_native(this.nativePointer, value.value, value.quality, value.timestamp, index, mode.toType());
    }

    @Override
    public void update(Counter value, int index, EventMode mode)
    {
        this.update_counter_native(this.nativePointer, value.value, value.quality, value.timestamp, index, mode.toType());
    }

    @Override
    public void update(FrozenCounter value, int index, EventMode mode)
    {
        this.update_frozen_counter_native(this.nativePointer, value.value, value.quality, value.timestamp, index, mode.toType());
    }

    @Override
    public void update(BinaryOutputStatus value, int index, EventMode mode)
    {
        this.update_bo_status_native(this.nativePointer, value.value, value.quality, value.timestamp, index, mode.toType());
    }

    @Override
    public void update(AnalogOutputStatus value, int index, EventMode mode)
    {
        this.update_ao_status_native(this.nativePointer, value.value, value.quality, value.timestamp, index, mode.toType());
    }

    private native long create_instance_native();
    private native void destroy_instance_native(long nativePointer);

    private native void update_binary_native(long nativePointer, boolean value, byte flags, long time, int index, int mode);
    private native void update_double_binary_native(long nativePointer, int value, byte flags, long time, int index, int mode);
    private native void update_analog_native(long nativePointer, double value, byte flags, long time, int index, int mode);
    private native void update_counter_native(long nativePointer, long value, byte flags, long time, int index, int mode);
    private native void update_frozen_counter_native(long nativePointer, long value, byte flags, long time, int index, int mode);
    private native void update_bo_status_native(long nativePointer, boolean value, byte flags, long time, int index, int mode);
    private native void update_ao_status_native(long nativePointer, double value, byte flags, long time, int index, int mode);
}
