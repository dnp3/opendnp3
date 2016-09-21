package com.automatak.dnp3;

public class IINField
{
    public enum LSB
    {
        ALL_STATIONS(1 << 0),
        CLASS1_EVENTS(1 << 1),
        CLASS2_EVENTS(1 << 2),
        CLASS3_EVENTS(1 << 3),
        NEED_TIME(1 << 4),
        LOCAL_CONTROL(1 << 5),
        DEVICE_TROUBLE(1 << 6),
        DEVICE_RESTART(1 << 7);

        public final int id;

        public boolean isSet(byte value)
        {
            return (value & id) != 0;
        }

        LSB(int id)
        {
            this.id = id;
        }
    }

    public enum MSB
    {
        FUNC_NOT_SUPPORTED(1 << 0),
        OBJECT_UNKNOWN(1 << 1),
        PARAM_ERROR(1 << 2),
        EVENT_BUFFER_OVERFLOW(1 << 3),
        ALREADY_EXECUTING(1 << 4),
        CONFIG_CORRUPT(1 << 5),
        RESERVED1(1 << 6),
        RESERVED2(1 << 7);

        public final int id;

        public boolean isSet(byte value)
        {
            return (value & id) != 0;
        }

        MSB(int id)
        {
            this.id = id;
        }
    }

    public final byte lsb;
    public final byte msb;

    public IINField(byte lsb, byte msb)
    {
        this.lsb = lsb;
        this.msb = msb;
    }

    public boolean isSet(LSB bit)
    {
        return bit.isSet(lsb);
    }

    public boolean isSet(MSB bit)
    {
        return bit.isSet(msb);
    }


}
