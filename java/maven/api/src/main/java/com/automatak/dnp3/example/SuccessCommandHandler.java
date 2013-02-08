package com.automatak.dnp3.example;

import com.automatak.dnp3.CommandStatus;

/**
 * Singleton that return SUCCESS for all command requests it receives
 */
public class SuccessCommandHandler extends ConstantCommandHandler {
    private static SuccessCommandHandler ourInstance = new SuccessCommandHandler();

    public static SuccessCommandHandler getInstance() {
        return ourInstance;
    }

    private SuccessCommandHandler() {
        super(CommandStatus.SUCCESS);
    }
}
