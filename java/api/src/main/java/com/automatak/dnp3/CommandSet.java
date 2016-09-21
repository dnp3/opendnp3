package com.automatak.dnp3;

/// <summary>
/// A command set collects headers and can replay their values back to a builder
/// </summary>
public class CommandSet implements CommandHeaders
{
        private final Iterable<CommandHeaders> headers;

        private CommandSet(Iterable<CommandHeaders> headers)
        {
            this.headers=headers;
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
