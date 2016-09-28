package com.automatak.dnp3.mock;

import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.AssignClassType;
import com.automatak.dnp3.enums.LinkStatus;
import com.automatak.dnp3.enums.MasterTaskType;
import com.automatak.dnp3.enums.PointClass;

import java.util.Collections;

public class DefaultOutstationApplication implements OutstationApplication
{
    private static DefaultOutstationApplication instance = new DefaultOutstationApplication();

    public static OutstationApplication getInstance() {
        return instance;
    }

    private DefaultOutstationApplication() {}

    @Override
    public boolean supportsWriteAbsoluteTime() { return false; }

    @Override
    public boolean writeAbsoluteTime(long msSinceEpoch) { return false; }

    @Override
    public boolean supportsAssignClass(){ return false; }

    @Override
    public void recordClassAssignment(AssignClassType type, PointClass clazz, int start, int stop) {}

    @Override
    public ApplicationIIN getApplicationIIN() { return ApplicationIIN.none; }

    @Override
    public void onStateChange(LinkStatus value) {}

    @Override
    public void onKeepAliveInitiated() {}

    @Override
    public void onKeepAliveFailure(){}

    @Override
    public void onKeepAliveSuccess(){}


}
