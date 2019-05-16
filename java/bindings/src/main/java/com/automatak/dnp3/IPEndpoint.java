package com.automatak.dnp3;

/**
 * An IP endpoint
 */
public class IPEndpoint
{
    public final String address;
    public final int port;

    public IPEndpoint(String address, int port)
    {
        this.address = address;
        this.port = port;
    }

    @Override
    public String toString()
    {
        return String.format("%s - %d", address, port);
    }
}
