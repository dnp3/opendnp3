package com.automatak.dnp3;

/**
 * Represents the set of IIN bits that can be controlled by the application
 */
public class ApplicationIIN
{
    public ApplicationIIN(
            boolean needTime,
            boolean localControl,
            boolean deviceTrouble,
            boolean configCorrupt)
    {
        this.needTime = needTime;
        this.localControl = localControl;
        this.deviceTrouble = deviceTrouble;
        this.configCorrupt =  configCorrupt;
    }

    public static ApplicationIIN none = new ApplicationIIN(false, false, false, false);

    public final boolean needTime;
    public final boolean localControl;
    public final boolean deviceTrouble;
    public final boolean configCorrupt;
}