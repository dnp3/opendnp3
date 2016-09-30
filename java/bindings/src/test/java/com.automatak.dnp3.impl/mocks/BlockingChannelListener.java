package com.automatak.dnp3.impl.mocks;

import com.automatak.dnp3.ChannelListener;
import com.automatak.dnp3.enums.ChannelState;

import java.time.Duration;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;


public class BlockingChannelListener implements ChannelListener {

    BlockingQueue<ChannelState> states = new LinkedBlockingQueue<>();

    public void waitFor(ChannelState state, Duration duration) {
        try {
            ChannelState cs = states.poll(duration.toMillis(), TimeUnit.MILLISECONDS);
            if(cs == null) throw new RuntimeException("Timeout waiting for state: " + state);
            if(cs != state) waitFor(state, duration);
        }
        catch(InterruptedException ex)
        {
            throw new RuntimeException(ex);
        }
    }

    @Override
    public void onStateChange(ChannelState state)
    {
        states.add(state);
    }

}
