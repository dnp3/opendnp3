package com.automatak.dnp3.impl;


import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.EventMode;

public class ChangeSetImpl extends Database {


    void update(BinaryInput value, int index, EventMode mode);


    void update(DoubleBitBinaryInput value, int index, EventMode mode);


    void update(AnalogInput value, int index, EventMode mode);


    void update(Counter value, int index, EventMode mode);


    void update(FrozenCounter value, int index, EventMode mode);


    void update(BinaryOutputStatus value, int index, EventMode mode);

    
    void update(AnalogOutputStatus value, int index, EventMode mode);
}
