package com.automatak.dnp3;

import com.automatak.dnp3.enums.TaskCompletion;

import java.util.Iterator;

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
        Iterator<CommandPointResult> iter = results.iterator();
        StringBuilder sb = new StringBuilder();

        while (iter.hasNext())
        {
            sb.append(iter.next());
            sb.append(iter.hasNext() ? ", " : "");
        }

        return String.format("Summary: %s Results: { %s }", summary, sb.toString());
    }

    public final TaskCompletion summary;
    public final Iterable<CommandPointResult> results;
}