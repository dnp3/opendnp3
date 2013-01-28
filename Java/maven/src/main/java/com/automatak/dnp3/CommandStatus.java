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
};

