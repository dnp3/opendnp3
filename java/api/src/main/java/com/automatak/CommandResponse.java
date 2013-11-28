package com.automatak;

import com.automatak.dnp3.CommandResult;
import com.automatak.dnp3.CommandStatus;

/**
 * Aggregate command response type.
 *
 * Check the result before checking the status.
 * Status is only valid if response is RESPONSE_OK
 */
public class CommandResponse {

    public CommandResponse(CommandResult result, CommandStatus status)
    {
        this.result = result;
        this.status = status;
    }

    public CommandResult getResult() {
        return result;
    }

    public CommandStatus getStatus() {
        return status;
    }

    private final CommandResult result;
    private final CommandStatus status;

    @Override
    public String toString()
    {
        if(result == CommandResult.RESPONSE_OK) return "Response: " + status.toString();
        else return "Failure: " + result.toString();
    }
}
