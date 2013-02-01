package com.automatak.dnp3;

public interface Master {
    CommandProcessor getCommandProcessor();
    void shutdown();
}
