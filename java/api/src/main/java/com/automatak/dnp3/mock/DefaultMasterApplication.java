package com.automatak.dnp3.mock;

import com.automatak.dnp3.ClassAssignment;
import com.automatak.dnp3.IINField;
import com.automatak.dnp3.MasterApplication;
import com.automatak.dnp3.TaskInfo;
import com.automatak.dnp3.enums.LinkStatus;
import com.automatak.dnp3.enums.MasterTaskType;


import java.util.Collections;

public class DefaultMasterApplication implements MasterApplication
{
    private static DefaultMasterApplication instance = new DefaultMasterApplication();

    public static MasterApplication getInstance() {
        return instance;
    }

    private DefaultMasterApplication() {}


    public long getMillisecondsSinceEpoch()
    {
        return System.currentTimeMillis();
    }

    public void onReceiveIIN(IINField iin)
    {}

    public void onTaskStart(MasterTaskType type, int userId)
    {}

    public void onTaskComplete(TaskInfo info)
    {}

    public boolean assignClassDuringStartup()
    {
        return false;
    }

    public Iterable<ClassAssignment> getClassAssignments()
    {
        return Collections.emptyList();
    }

    public void OnStateChange(LinkStatus value){}

    public void OnKeepAliveInitiated(){}

    public void OnKeepAliveFailure(){}

    public void OnKeepAliveSuccess(){}

}
