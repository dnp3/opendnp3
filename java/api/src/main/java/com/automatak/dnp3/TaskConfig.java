package com.automatak.dnp3;

public class TaskConfig
{
    public final TaskId taskId;
    public final TaskCallback callback;     // may be null

    public TaskConfig(TaskId taskId, TaskCallback callback)
    {
        this.taskId = taskId;
        this.callback = callback;
    }

    public static TaskConfig getDefault()
    {
        return new TaskConfig(TaskId.undefined(), null);
    }
}
