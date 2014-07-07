using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    interface IDNP3Config
    {

        IEnumerable<KeyValuePair<string, DatabaseTemplate>> Templates
        {
            get;
        }
    }
}
