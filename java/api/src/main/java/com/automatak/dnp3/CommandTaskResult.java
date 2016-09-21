package com.automatak.dnp3;

import com.automatak.dnp3.enums.TaskCompletion;

/**
 * Result type returned for CommandSet based command requests
 */
public class CommandTaskResult
{
    public CommandTaskResult(com.automatak.dnp3.enums.TaskCompletion summary, Iterable<CommandPointResult> results)
    {
        this.summary = summary;
        this.results = results;
    }

/*    TODO
    @Override
    public String toString()
    {
        return String.format("Summary: %s Results: [%s]", summary, String.join("; ", results));
    }
*/

    public final TaskCompletion summary;
    public final Iterable<CommandPointResult> results;
}