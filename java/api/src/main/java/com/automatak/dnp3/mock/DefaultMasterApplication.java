package com.automatak.dnp3.mock;

import com.automatak.dnp3.*;
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

    @Override
    public long getMillisecondsSinceEpoch()
    {
        return System.currentTimeMillis();
    }

    @Override
    public void onReceiveIIN(IINField iin)
    {}

    @Override
    public void onTaskStart(MasterTaskType type, TaskId userId)
    {}

    @Override
    public void onTaskComplete(TaskInfo info)
    {}

    @Override
    public boolean assignClassDuringStartup()
    {
        return false;
    }

    @Override
    public Iterable<ClassAssignment> getClassAssignments()
    {
        return Collections.emptyList();
    }

    @Override
    public void onStateChange(LinkStatus value){}

    @Override
    public void onKeepAliveInitiated(){}

    @Override
    public void onKeepAliveFailure(){}

    @Override
    public void onKeepAliveSuccess(){}

}
