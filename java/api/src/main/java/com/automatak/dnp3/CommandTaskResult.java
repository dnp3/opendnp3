package com.automatak.dnp3;

import com.automatak.dnp3.enums.TaskCompletion;

/**
 * Result type returned for CommandSet based command requests
 */
public class CommandTaskResult
{
    public CommandTaskResult(TaskCompletion summary, Iterable<CommandPointResult> results)
    {
        this.summary = summary;
        this.results = results;
    }

    @Override
    public String toString()
    {
        return String.format("Summary: %s Results: [%s]", summary, "TODO");
    }

    public final TaskCompletion summary;
    public final Iterable<CommandPointResult> results;
}