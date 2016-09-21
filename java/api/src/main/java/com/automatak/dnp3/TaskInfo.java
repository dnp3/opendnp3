package com.automatak.dnp3;

import com.automatak.dnp3.enums.MasterTaskType;
import com.automatak.dnp3.enums.TaskCompletion;

public class TaskInfo
{
    public TaskInfo(MasterTaskType type, TaskCompletion result, TaskId taskid)
    {
        this.type = type;
        this.result = result;
        this.taskid = taskid;
    }

    public final MasterTaskType type;
    public final TaskCompletion result;
    public final TaskId taskid;
}
