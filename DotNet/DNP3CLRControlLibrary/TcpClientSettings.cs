using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace TestSetControlLibrary
{
    public class TcpSettings
    {
        public TcpSettings(String name, String address, UInt16 port, FilterLevel level, UInt64 timeout)
        {
            this.name = name;
            this.address = address;
            this.port = port;
            this.level = level;
            this.timeout = timeout;
        }

        public readonly String name;
        public readonly String address;
        public readonly UInt16 port;
        public readonly FilterLevel level;
        public readonly UInt64 timeout;

        public override string ToString()
        {
            var lines = new String[] 
                {"name: " + name, 
                  "address: " + address, 
                  "port: " + port, 
                  "level: " + level, 
                  "timeout: " + timeout
                };

            return String.Join(Environment.NewLine, lines);
        }
    }
}
