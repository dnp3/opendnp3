package com.automatak.dnp3.impl;

import com.automatak.dnp3.ChannelState;
import com.automatak.dnp3.ChannelStateListener;

/**
 * Proxy class that makes marshalling the channel state enumeration easier
 */
class ChannelStateProxy {

    private final ChannelStateListener listener;

    public ChannelStateProxy(ChannelStateListener listener)
    {
        this.listener = listener;
    }

    void onStateChange(int state)
    {
        listener.onStateChange(ChannelState.fromInt(state));
    }


}
