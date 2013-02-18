package com.automatak.dnp3.impl;

import com.automatak.dnp3.StackState;
import com.automatak.dnp3.StackStateListener;

/**
 * Proxy class that makes marshalling the stack state enumeration easier
 */
class StackStateProxy {

    private final StackStateListener listener;

    public StackStateProxy(StackStateListener listener)
    {
        this.listener = listener;
    }

    void onStateChange(int state)
    {
        listener.onStateChange(StackState.fromInt(state));
    }

}
