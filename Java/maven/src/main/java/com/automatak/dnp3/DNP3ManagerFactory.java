package com.automatak.dnp3;

import com.automatak.dnp3.impl.ManagerImpl;

public class DNP3ManagerFactory {

    public static DNP3Manager createManager(int concurrency)
    {
        return new ManagerImpl(concurrency);
    }

}
