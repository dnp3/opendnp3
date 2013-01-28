package com.automatak.dnp3;

public enum ControlCode {
    CC_NULL(0),			    //  illegal command code (used internally)
    CC_PULSE(0x01),		    //  'push-button' interface, can only be pressed one way (reset button on pedometer)
    CC_LATCH_ON(0x03),		//  'light-switch' moved to the ON position
    CC_LATCH_OFF(0x04),	    //  'light-switch' moved to the OFF position
    CC_PULSE_CLOSE(0x41),	//  'doorbell' that rings while the button is depressed
    CC_PULSE_TRIP(0x81),	//  'doorbell' that stops ringing (is normally on) while depreseed
    CC_UNDEFINED(0xFF);		//   undefined command (used by DNP standard)

    private final int id;

    private ControlCode(int id)
    {
        this.id = id;
    }
};
