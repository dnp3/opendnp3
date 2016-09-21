package com.automatak.dnp3;

import com.automatak.dnp3.enums.TaskCompletion;

/**
 * Interface that receives events from user-defined tasks
 */
public interface TaskCallback
{
    /**
     * Called when the task begins to run
     */
    void onStart();

    /**
     * Called when the task succeeds or fails
     * @param result
     */
    void onComplete(TaskCompletion result);

    /**
     * Called when the task will never run again
     */
    void onDestroyed();
}
