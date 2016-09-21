package com.automatak.dnp3;

import com.automatak.dnp3.enums.LinkStatus;

/**
 * Various informational callbacks that can be received from the link layer
 */
public interface LinkStatusListener
{
    /// Called when a the reset/unreset status of the link layer changes
    void OnStateChange(LinkStatus value);

    /// Called when a keep alive message is transmitted
    void OnKeepAliveInitiated();

    /// Called when a keep alive message (request link status) receives no response
    void OnKeepAliveFailure();

    /// Called when a keep alive message receives a valid response
    void OnKeepAliveSuccess();
}
