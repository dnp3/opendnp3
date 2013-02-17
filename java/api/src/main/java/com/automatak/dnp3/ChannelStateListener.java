package com.automatak.dnp3;

/**
 * Callback interface invoked when the state of a communication channel changes
 */
public interface ChannelStateListener {

    void onStateChange(ChannelState state);
}
