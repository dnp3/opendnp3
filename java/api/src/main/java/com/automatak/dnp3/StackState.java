package com.automatak.dnp3;

/**
 * Enumeration describing the communication health of a master or outstation
 */
public enum StackState
{
    /**
     * The stack is successfully communicating with the other side
     */
    COMMS_UP,

    /**
     * The stack is not communicating well with the other side
     */
    COMMS_DOWN,

    /**
     * The communication health is unknown
     */
    UNKNOWN;

    public static StackState fromInt(int state)
    {
        switch(state)
        {
            case(0):
                return COMMS_UP;
            case(1):
                return COMMS_DOWN;
            default:
                return UNKNOWN;
        }
    }
}
