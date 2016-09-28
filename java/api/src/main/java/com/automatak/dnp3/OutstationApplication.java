package com.automatak.dnp3;

import com.automatak.dnp3.enums.AssignClassType;
import com.automatak.dnp3.enums.PointClass;

/**
 * Outstation application code implements this interface to interface with the stack
 */
public interface OutstationApplication extends LinkStatusListener {

    /**
     * @return true of the outstation should allow absolute time to be written
     */
    boolean supportsWriteAbsoluteTime();

    /**
     * Called when the master writes the time
     * @param msSinceEpoch milliseconds since Unix epoch
     * @return true, if the wrote is successful
     */
    boolean writeAbsoluteTime(long msSinceEpoch);

    /**
     * @return true if the outstation supports assigning class
     */
    boolean supportsAssignClass();

    /**
     * Called when the master assigns class to a particular type of measurements
     * @param type the type of the assignment
     * @param clazz the class assigned
     * @param start the start of the measurement range
     * @param stop the end of the measurement range
     */
    void recordClassAssignment(AssignClassType type, PointClass clazz, int start, int stop);

    /**
     * @return Application controlled IIN bits
     */
    ApplicationIIN getApplicationIIN();
}
