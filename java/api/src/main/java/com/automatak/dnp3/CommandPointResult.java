package com.automatak.dnp3;

import com.automatak.dnp3.enums.CommandPointState;
import com.automatak.dnp3.enums.CommandStatus;

/**
 * Aggregate result of command operation.  Check the Result before checking the status.
 * Status is only valid when Result == RESPONSE_OK
 */
public class CommandPointResult
{
    public CommandPointResult(int headerIndex, int requestIndex, CommandPointState state, CommandStatus status)
    {
        this.headerIndex = headerIndex;
        this.requestIndex = requestIndex;
        this.state = state;
        this.status = status;
    }

    @Override
    public String toString()
    {
        return String.format("Result(header: %d index: %d State: %s Status: %s)", headerIndex, requestIndex, state, status);
    }


    final int headerIndex;
    final int requestIndex;
    final CommandPointState state;
    final CommandStatus status;
}
