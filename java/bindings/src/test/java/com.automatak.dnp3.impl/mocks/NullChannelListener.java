package com.automatak.dnp3.impl.mocks;

import com.automatak.dnp3.ChannelListener;
import com.automatak.dnp3.enums.ChannelState;


public class NullChannelListener implements ChannelListener {

    @Override
    public void onStateChange(ChannelState state) {}

}
