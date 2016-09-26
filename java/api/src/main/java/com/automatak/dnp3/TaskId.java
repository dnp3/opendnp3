package com.automatak.dnp3;

public class TaskId
{
    public final int id;
    public final boolean isValid;

    public TaskId(int id, boolean isDefined)
    {
        this.id = id;
        this.isValid = isDefined;
    }

    public static TaskId defined(int id)
    {
        return new TaskId(id, true);
    }

    public static TaskId undefined()
    {
        return new TaskId(-1, false);
    }
}
