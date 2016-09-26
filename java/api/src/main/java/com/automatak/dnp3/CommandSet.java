package com.automatak.dnp3;

import java.util.ArrayList;

/// <summary>
/// A command set collects headers and can replay their values back to a builder
/// </summary>
public class CommandSet implements CommandHeaders
{
        private final ArrayList<CommandHeaders> headers = new ArrayList<>();

        public CommandSet(Iterable<CommandHeaders> headers)
        {
            headers.forEach((h) -> this.headers.add(h));
        }

        public static CommandHeaders from(Iterable<CommandHeaders> headers)
        {
            return new CommandSet(headers);
        }

        @Override
        public void build(CommandBuilder builder)
        {
            for(CommandHeaders header : headers)
            {
                header.build(builder);
            }
        }
}
