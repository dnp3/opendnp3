using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    class DNP3Config : IDNP3Config
    {
        IDictionary<string, DatabaseTemplate> templates = new Dictionary<string, DatabaseTemplate>();

        IEnumerable<KeyValuePair<string, DatabaseTemplate>> IDNP3Config.Templates
        {
            get 
            {
                return templates.Select(x => x);
            }
        }
    }
}
