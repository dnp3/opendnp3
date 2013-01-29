package com.automatak.dnp3;


public enum CommandStatus {

    SUCCESS(0),		    // command was successfully received and handled
    TIMEOUT(1),		    // command timeout before completing
    NO_SELECT(2),		// command requires being selected before operate, configuration issue
    FORMAT_ERROR(3),	// bad control code or timing values
    NOT_SUPPORTED(4),	// command is not implemented
    ALREADY_ACTIVE(5),	// command is already in progress or its already in that mode
    HARDWARE_ERROR(6),	// something is stopping the command, often a local/remote interlock
    LOCAL(7),			// the function governed by the control is in local only control
    TOO_MANY_OPS (8),	// the command has been done too often and has been throttled
    NOT_AUTHORIZED(9),	// the command was rejected because the device denied it or an RTU intercepted it
    UNDEFINED(127);	    // 10 to 126 are currently reserved

    private final int id;

    private CommandStatus(int id)
    {
        this.id = id;
    }

    public int toInt()
    {
        return id;
    }

    public static CommandStatus fromInt(int id)
    {
        switch(id)
        {
            case(0): return SUCCESS;
            case(1): return TIMEOUT;
            case(2): return NO_SELECT;
            case(3): return FORMAT_ERROR;
            case(4): return NOT_SUPPORTED;
            case(5): return ALREADY_ACTIVE;
            case(6): return HARDWARE_ERROR;
            case(7): return LOCAL;
            case(8): return TOO_MANY_OPS;
            case(9): return NOT_AUTHORIZED;
            default: return UNDEFINED;
        }
    }
};

