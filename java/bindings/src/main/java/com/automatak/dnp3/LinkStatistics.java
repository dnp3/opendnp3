package com.automatak.dnp3;


/**
 * Combined channel and link parser statistics
 */
public class LinkStatistics {

    public LinkStatistics(ChannelStatistics channel, ParserStatistics parser)
    {
        this.channel = channel;
        this.parser = parser;
    }

    final ChannelStatistics channel;
    final ParserStatistics parser;
}
