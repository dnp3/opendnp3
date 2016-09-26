package com.automatak.dnp3;

/**
 * A CommandHeaders is something that can send some info to a CommandBuilder
 */
public interface CommandHeaders
{
    void build(CommandBuilder builder);
};