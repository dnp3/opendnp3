package com.automatak.dnp3;

import com.automatak.dnp3.enums.MasterTaskType;

/**
 * Master application code implements this interface to interface with stack
 */
public interface MasterApplication extends LinkStatusListener {

    /**
     * Get UTC time
     *
     * @return a count of milliseconds since Unix epoch for the purposes of time syncing
     */
    long getMillisecondsSinceEpoch();

    /**
     * Called whenever an IIN field is received in an ASDU from the outstation
     * @param iin the received IIN field
     */
    void onReceiveIIN(IINField iin);

    /**
     * Called whenever a task starts
     * @param type The type of the task
     * @param userId The user assigned id
     */
    void onTaskStart(MasterTaskType type, int userId);

    /**
     * Task completion notification for built-in and user defined tasks
     * @param info information about the complete task
     */
    void onTaskComplete(TaskInfo info);

    /**
     * Tells the master whether to assign class on startup
     * @return if true, the master will assign classes during startup
     */
    boolean assignClassDuringStartup();

    /**
     * Returns a list of class assignments to perform during startup or restart
     * Only called if AssignClassDuringStartup returns true
     * @return An enumerable collection of class assignments
     */
    Iterable<ClassAssignment> getClassAssignments();
}
